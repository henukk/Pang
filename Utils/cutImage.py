from PIL import Image

def recortar_imagen(archivo_entrada, tamaño, offset, archivo_salida):
    """
    Recorta una imagen basada en el tamaño y offset especificados.

    :param archivo_entrada: Ruta del archivo de imagen de entrada.
    :param tamaño: Una tupla (ancho, alto) para el tamaño del recorte.
    :param offset: Una tupla (x, y) para el offset del recorte desde la esquina superior izquierda.
    :param archivo_salida: Ruta del archivo de imagen de salida.
    """
    try:
        # Abrir la imagen original
        with Image.open(archivo_entrada) as img:
            # Calcular la región de recorte
            x, y = offset
            ancho, alto = tamaño
            area_recorte = (x, y, x + ancho, y + alto)

            # Recortar la imagen
            imagen_recortada = img.crop(area_recorte)

            # Guardar la imagen recortada
            imagen_recortada.save(archivo_salida)

        print(f"Imagen recortada guardada en {archivo_salida}")
    except Exception as e:
        print(f"Error al recortar la imagen: {e}")
    
#   Cut image
#for i in range(50):
#    if(i % 2 == 0):
#        recortar_imagen("./Assets/Foreground.png", (384, 208), (8, 216 * i/2 + 44), "./Assets/Foreground/stage" + str(int(i/3)+1) + "-" + str(i%3+1) + ".png")
#    else:
#        recortar_imagen("./Assets/Foreground.png", (384, 208), (400, 216 * int(i/2) + 44), "./Assets/Foreground/stage" + str(int(i/3)+1) + "-" + str(i%3+1) + ".png")
