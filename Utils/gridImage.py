from PIL import Image, ImageDraw

def agregar_rejilla(file_name):
    # Configuraciones de la imagen y la rejilla
    tamaño_celda = 8
    tamaño_linea_gruesa = 4 * tamaño_celda
    grosor_linea_fina = 1
    grosor_linea_gruesa = 1

    # Cargar o crear la imagen
    try:
        imagen = Image.open(file_name)
    except FileNotFoundError:
        print(f"No se pudo encontrar el archivo {file_name}.")
        return

    ancho, alto = imagen.size
    dibujar = ImageDraw.Draw(imagen)

    # Dibujar las líneas de la rejilla
    # Líneas finas discontinuas 2 pixeles pintados, 2 pixeles en blanco
    # Líneas gruesas continuas de 2 pixeles de ancho
    for x in range(0, ancho, tamaño_celda):
        if x % tamaño_linea_gruesa == 0:
            # Línea gruesa
            dibujar.line((x, 0, x, alto), fill="black", width=grosor_linea_gruesa)
        else:
            # Línea fina discontinua
            for y in range(0, alto, 4):
                dibujar.line((x, y, x, min(y + 2, alto)), fill="black", width=grosor_linea_fina)

    for y in range(0, alto, tamaño_celda):
        if y % tamaño_linea_gruesa == 0:
            # Línea gruesa
            dibujar.line((0, y, ancho, y), fill="black", width=grosor_linea_gruesa)
        else:
            # Línea fina discontinua
            for x in range(0, ancho, 4):
                dibujar.line((x, y, min(x + 2, ancho), y), fill="black", width=grosor_linea_fina)

    # Guardar la imagen
    new_file_name = file_name.rsplit('.', 1)[0] + "_grid.png"
    imagen.save(new_file_name)
    print(f"Imagen guardada como {new_file_name}")

#   make Grid image
#for i in range(50):
#    agregar_rejilla("./Assets/Foreground/stage" + str(int(i/3)+1) + "-" + str(i%3+1) + ".png")