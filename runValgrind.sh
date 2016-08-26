#! /bin/bash

valgrind --track-origins=yes --dsymutil=yes ../bin/corrfunc

valgrind --track-origins=yes --dsymutil=yes --leak-check=full  ../bin/corrfunc

