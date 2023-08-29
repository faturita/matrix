/*! \page ProgrammingTroubleshooting Troubleshooting Programming
\ref TableOfContents : \ref IntroductionPage

\section GeneralDebugSuggestions General Debugging Suggestions
- Start from the beginning of the error listing. Errors at the beginning usually cause multiple related errors 
propagated throughout the build. The first error is usual the culprit. Fix these first couple of errors then 
attempt to recompile.
- Check to see if the error was a compile error or a linker error. Compiler errors give more, but sometimes cryptic 
descriptions, whereas linker errors usually refer to unfound functions. Linker errors are resolved by checking that 
the appropriate object file (.o) is linked in the make command, and the function is defined in the appropriate .cpp file. 

*/