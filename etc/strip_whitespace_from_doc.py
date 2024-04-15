"""
Note that this requires a working version of rstfmt
"""
import sys, os, re, subprocess

temp_rst_file = "temp_doc_string_rst.rst"
files = sys.argv[1:]

# Lines that have empty space before keywords are bad.
bad_doc = re.compile(r"(?:\s+:(param))|(?:\s+:(return))|(?:\s+:(r?type))")


for file in files:
    is_doc_block = False    # Is the current line part of a docstring
    is_comment = False      # Is the current line in a comment
    is_bad_block = False    # Does the current doc block contain a bad line
    
    blocks = []
    block = []

    with open(file, "r") as f:
        content = f.readlines()

    # Find the doc blocks that aren't commented out
    for i, line in enumerate(content):
        if "/*" in line:
            is_comment = True
        if "*/" in line:
            is_comment = False
        if is_comment:
            continue

        if line.count("pbdoc") % 2 == 1:
            block.append(i)
            if is_doc_block:
                if is_bad_block:
                    blocks.append(block)
                block = []
            else:
                is_bad_block = False
            is_doc_block = not is_doc_block
        
        if is_doc_block and bad_doc.match(line):
            is_bad_block = True

    # Work from back to front so that we preserve line numbers
    for start, end in blocks[::-1]:
        with open(temp_rst_file, "w") as f:
            for i in range(start + 1, end):
                f.write(content[i])
        
        # Try and format the rst
        try:
            subprocess.run(
                ["rstfmt", temp_rst_file],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                check=True,
            )
        except subprocess.CalledProcessError as e:
            print(f"Formatting failed for:\n{"".join(content[start:end])}\n in {file}")

        
        with open(temp_rst_file, "r") as f:
            formatted_content = f.readlines()
        
        for i, line in enumerate(formatted_content):
            formatted_content[i] = re.sub(r'^[^\S\n]{3}', "", line)
        
        # Replace bad rst with good rst
        del content[start + 1 : end]
        content[start + 1 : start + 1] = formatted_content

    with open(file, "w") as f:
        f.writelines(content)

if len(blocks) == 0:
    print("No changes made")

try:
    os.remove(temp_rst_file)
except OSError:
    pass

