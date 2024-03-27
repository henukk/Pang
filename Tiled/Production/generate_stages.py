import os

# Lista de strings
nombres_de_archivo = ['stage1-1', 'stage1-2', 'stage1-3', 'stage2-1', 'stage2-2', 'stage2-3', 'stage3-1', 'stage3-2', 'stage3-3', 'stage4-1', 'stage4-2', 'stage4-3', 'stage5-1', 'stage5-2', 'stage5-3', 'stage6-1', 'stage6-2', 'stage6-3', 'stage7-1', 'stage7-2', 'stage7-3', 'stage8-1', 'stage8-2', 'stage8-3', 'stage9-1', 'stage9-2', 'stage9-3', 'stage10-1', 'stage10-2', 'stage10-3', 'stage11-1', 'stage11-2', 'stage11-3', 'stage12-1', 'stage12-2', 'stage12-3', 'stage13-1', 'stage13-2', 'stage13-3', 'stage14-1', 'stage14-2', 'stage14-3', 'stage15-1', 'stage15-2', 'stage15-3', 'stage16-1', 'stage16-2', 'stage16-3', 'stage17-1', 'stage17-2']

# Contenidos X y Y
header_1 = "TILEMAP\n48 30 -\n8 -\nbackgrounds/"
header_2 = ".png -\nimages/Sprites.png -\n32 32 -\n"
footer = "232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232\n232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232\n232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232\n232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232\n"

# Procesar cada archivo en la lista
for nombre in nombres_de_archivo:
    nombre_archivo = nombre + '.csv'

    # Verificar si el archivo existe
    if os.path.isfile(nombre_archivo):
        # Leer el contenido del archivo existente
        with open(nombre_archivo, 'r') as archivo:
            contenido_original = archivo.read()

        # Crear o sobrescribir un nuevo archivo con el contenido combinado
        nuevo_nombre_archivo = nombre + ".txt"
        with open(nuevo_nombre_archivo, 'w') as nuevo_archivo:
            nuevo_archivo.write(header_1 + nombre + header_2 + contenido_original + footer)
        print(f'Archivo procesado: {nuevo_nombre_archivo}')
    else:
        print(f'Archivo no encontrado: {nombre_archivo}')