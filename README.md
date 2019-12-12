# String Mapping

### Problem Statement
There are K strings Xi from the vocabulary V. Each string Xi has length Ni. The goal is to map the strings to each other. One can think of this in two steps – conversion and matching.

Conversion is a function F that takes in a string and returns another string. All F(Xi)s have the same length N. N is greater than equal to all Nis. The function F is only allowed to make one change to the original string – it can introduce dashes. It can introduce any number of dashes at any position. The conversion cost of X to F(X) is CC number of dashes, CC being a constant.

Once all strings have been converted the matching step just matches characters at each position. The matching cost between two characters is given by a symmetric function MC(c1, c2) where c1 and c2 are two characters ϵ V U {-}. Matching cost of two strings is the sum of matching costs of their conversions at each position. Finally, the matching cost of K strings is the sum of pairwise matching costs between each pair.

### Input Format
Time (in mins)
|V|
V
K
X1
X2
…
CC
MC
#
(Refer to *in.txt* for an example)

### Output Format
F(X1)
F(X2)
…

### Running the code
1. To compile
```
./compile.sh
```

2. To run
```
./run.sh input.txt output.txt
```

3. To check if the output format is according to the specifications
```
python format_checker.py input.txt output.txt
```