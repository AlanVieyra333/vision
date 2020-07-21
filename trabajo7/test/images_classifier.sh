#!/bin/bash

echo "CIRCULOS:" > classifications.txt

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
    test/classifier.a ./images/circulo/imagen$i.png >> classifications.txt
done

echo "TRIANGULOS:" >> classifications.txt

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
    test/classifier.a ./images/triangulo/imagen$i.png >> classifications.txt
done