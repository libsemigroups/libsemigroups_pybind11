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
  silent '<,'>s/\(\d\)\(\d\+_w\)/\1, \2/ge
  silent '<,'>s/\<\(\d\)_w/\1]/ge
  silent '<,'>s/!/not /ge
  silent '<,'>s/\/\/.*$//ge
endfunction

map! <F1> <ESC>:call CatchCPPToPytest()<CR>i
map <F1> :call CatchCPPToPytest()<CR>
