Alternative IsDebuggerPresent
=============================
My version of IsDebuggerPresent is working like the normal one but my version is able to detect emulation/debugging software with one little trick. At first I'm checking for the IsDebuggerPresent flag in the PEB block, then I compare it with the API call. If nothing is found I set the IsDebuggerPresent flag to TRUE. Then I compare again the flag in the PEB and the API result and compare them both, if there are differences then it's a (bad) emulation software.
