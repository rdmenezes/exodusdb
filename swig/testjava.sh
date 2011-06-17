#!/bin/bash
set -e

test -f test.class && rm test.class && echo rm test.class

echo \
javac -cp java test.java
javac -cp java test.java

echo \
java -cp java:. -Djava.library.path=java test
java -cp java:. -Djava.library.path=java test
