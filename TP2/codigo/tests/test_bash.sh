#!/bin/bash

filtro=$1
implementacion=$2
parametro=$3

if [ -z "$filtro" ] || [ -z "$implementacion" ]; then
  echo "Tenes que ingresar al menos dos parametros."
  echo "Modo de uso:"
  echo "  $0 <filtro> <implementacion> <parametro(si es necesario)>"
  exit
fi

if [ $filtro != "colorizar" ] && [ $filtro != "smalltiles" ] && [ $filtro != "pixelar" ] && [ $filtro != "combinar" ] && [ $filtro != "rotar" ]; then
  echo "El filtro ingresado ($filtro) no es valido."
  echo "Modo de uso:"
  echo "  $0 <filtro> <implementacion> <parametro(si es necesario)>"
  exit
fi

if [ $implementacion != "asm" ] && [ $implementacion != "c" ]; then
  echo "La implementacion ingresada ($implementacion) no es valida."
  echo "Modo de uso:"
  echo "  $0 <filtro> <implementacion> <parametro(si es necesario)>"
  exit
fi

if ([ $filtro == "colorizar" ] || [ $filtro == "combinar" ]) && [ -z "$parametro" ]; then
  echo "Para este filtro ($filtro) debes ingresar un parametro."
  echo "Modo de uso:"
  echo "  $0 <filtro> <implementacion> <parametro(si es necesario)>"
  exit
fi

echo "Parametros ingresados"
echo $filtro
echo $implementacion
echo $parametro


sizes=("200x200" "204x204" "208x208" "256x256" "512x512" "1024x768" "1152x864" "1280x1024" "1440x1152" "1680x1152" "1920x1080")

i="1"
for var in "${sizes[@]}"; do
  # path="pwd"
  file_name=$(ls ./data/imagenes_a_testear/ | grep $var)
  mv data/imagenes_a_testear/$file_name data/imagenes_a_testear/$i.$file_name 
  i=$((i+1))  
done

i=0
if [ -z "$parametro" ]; then
  for filename in ./data/imagenes_a_testear/*.bmp; do
    if [ $i == 0 ]; then
      echo $filename ":" > $filtro.$implementacion.out
    else
      echo $filename ":" >> $filtro.$implementacion.out
    fi
    ./../build/tp2 -v -i $implementacion $filtro $filename | grep llamada | awk '{print $8}' >> $filtro.$implementacion.out
    i=$((i+1))
  done
else
  for filename in ./data/imagenes_a_testear/*.bmp; do
    if [ $i == 0 ]; then
      echo $filename ":" > $filtro.$implementacion.out
    else
      echo $filename ":" >> $filtro.$implementacion.out
    fi
    ./../build/tp2 -v -i $implementacion $filtro $filename $parametro | grep llamada | awk '{print $8}' >> $filtro.$implementacion.out
    i=$((i+1))
  done
fi

path=$(pwd)
for filename in $path/*.bmp; do
  rm $filename
done