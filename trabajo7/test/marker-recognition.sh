#!/bin/bash

pushd ../material/Reconoce
make
popd

echo "MARCADORES:" > classifications.txt

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
    echo $i >> classifications.txt
    ../material/Reconoce/reconoce ./images/marcador/imagen$i.png >> classifications.txt
done

# ../material/Reconoce/reconoce ./images/marcador/imagen16.png