from os.path import isfile
Import("env")
import re

assert isfile(".env")
try:
    with open(".env", "r") as f:
        lines = f.readlines()
        envs = []
        for line in lines:
            line = line.strip()
            if not line or line.startswith("#"):
                continue  # Ignorar líneas vacías o comentarios
            
            # Dividir en clave y valor
            key, value = line.split("=", 1)
            
            # Reemplazar caracteres no válidos para los nombres de macros
            sanitized_key = re.sub(r'[^a-zA-Z0-9_]', '_', key)
            
            # Crear una macro con el valor entre comillas para cadenas
            envs.append(f'-D{sanitized_key}="{value.strip()}"')

        # Agregar las macros a los BUILD_FLAGS
        env.Append(BUILD_FLAGS=envs)
except IOError:
    print("Error: no se pudo acceder al archivo .env")
