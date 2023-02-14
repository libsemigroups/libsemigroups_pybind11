function! CatchCPPToPytest()
  silent '<,'>s/{/[/ge
  silent '<,'>s/}/]/ge
  silent '<,'>s/::/./ge
  silent '<,'>s/true/True/ge
  silent '<,'>s/false/False/ge
  silent '<,'>s/;//ge
  silent '<,'>s/REQUIRE(\([^)]*\))/assert \1/ge
  silent '<,'>s/<[^>]*>//ge
  silent '<,'>s/REQUIRE_THROWS_AS(\([^,]\+\),\s\+LibsemigroupsException)/with pytest.raises(RuntimeError):\r        assert \1/ge
endfunction

function! DoxyToSphinx()
  silent '<,'>s/\/\/!//ge
  silent '<,'>s/^.*\\tparam.*$//ge
  silent '<,'>s/\\param\s\+\(\w\)\+\(.*\)$/:param \1: \2\r    :type \1: ??/ge
  silent '<,'>s/\\returns/:returns:/ge
  silent '<,'>s/\\throws LibsemigroupsException/:raises RuntimeError:/ge
  silent '<,'>s/`\{-1}/``/ge
  silent '<,'>s/`\{4}/``/ge
  silent '<,'>s/\\p\s\+\(\w\+\)/``\1``/ge
  silent '<,'>s/\\ref\s\+\(\w\+\)/:py:any:`\1`/ge

endfunction

map! <F1> <ESC>:call CatchCPPToPytest()<CR>i
map <F1> :call CatchCPPToPytest()<CR>
map! <F2> <ESC>:call DoxyToSphinx()<CR>i
map <F2> :call DoxyToSphinx()<CR>
