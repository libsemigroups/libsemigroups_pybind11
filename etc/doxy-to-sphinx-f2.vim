function! DoxyToSphinx()
  silent '<,'>s/\/\/!//ge
  silent '<,'>s/^.*\\tparam.*$//ge
  silent '<,'>s/\\param\s\+\(\w\+\)/:param \1:/ge
  silent '<,'>s/\\returns/:returns:/ge
  silent '<,'>s/\\throws LibsemigroupsException/:raises RuntimeError:/ge
  silent '<,'>s/`\{-1}/``/ge
  silent '<,'>s/`\{4}/``/ge
  silent '<,'>s/\\p\s\+\(\w\+\)/*\1*/ge
  silent '<,'>s/\\c\s\+\(\w\+\)/``\1``/ge
  silent '<,'>s/``true``/``True``/ge
  silent '<,'>s/``false``/``False``/ge
  silent '<,'>s/\\warning/.. warning::\r/ge
  silent '<,'>s/\\sa/.. seealso::\r/ge
  silent '<,'>s/\\complexity/:Complexity:\r       /ge
  silent '<,'>s/\\f\$\(.\{-}\)\\f\$/:math:`\1`/ge
  silent '<,'>s/:math:``\(.\{-}\)``/:math:`\1`/ge
  silent '<,'>s/``\(.\{-}\)``_/`\1`_/ge
  silent '<,'>s/\\ref_knuth_bendix/:any:`KnuthBendix`/ge
  silent '<,'>s/\\brief//ge
  silent '<,'>s/\\ref\s\+\([a-zA-z:]\+\)/:any:`\1`/ge
  silent '<,'>s/:any:``\(.\{-}\)``/:any:`\1`/ge
  silent '<,'>s/::/./ge
  silent '<,'>s/^\s*//ge
endfunction

map! <F2> <ESC>:call DoxyToSphinx()<CR>i
map <F2> :call DoxyToSphinx()<CR>
