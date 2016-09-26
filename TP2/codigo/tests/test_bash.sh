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

i=0
if [ -z "$parametro" ]; then
  for filename in ./data/imagenes_a_testear/*.bmp; do
    if [ $i == 0 ]; then
      echo $filename ":" > $filtro.$implementacion.out
    else
      echo $filename ":" >> $filtro.$implementacion.out
    fi
    ./../build/tp2 -v -i $implementacion $filtro $filename | grep totales | awk '{print $7}' >> $filtro.$implementacion.out
    i=$((i+1))
  done
else
  for filename in ./data/imagenes_a_testear/*.bmp; do
    if [ $i == 0 ]; then
      echo $filename ":" > $filtro.$implementacion.out
    else
      echo $filename ":" >> $filtro.$implementacion.out
    fi
    ./../build/tp2 -v -i $implementacion $filtro $filename $parametro | grep totales | awk '{print $7}' >> $filtro.$implementacion.out
    i=$((i+1))
  done
fi

for filename in ./*.bmp; do
  rm $filename
done