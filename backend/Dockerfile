# Usamos una imagen base oficial de OpenJDK
FROM openjdk:24-jdk-slim AS build


# Instala Maven
RUN apt-get update && apt-get install -y maven

# Establecemos el directorio de trabajo
WORKDIR /app

# Copiamos el archivo POM y las dependencias de Maven
COPY pom.xml .
RUN mvn dependency:go-offline -B

# Copiamos el código fuente del proyecto
COPY src /app/src

# Compilamos el WAR del proyecto
RUN mvn clean package -DskipTests

# Usamos una imagen de OpenJDK más ligera para la fase final
FROM openjdk:24-jdk-slim

# Establecemos el directorio de trabajo
WORKDIR /app

# Copiamos el WAR generado desde la fase anterior
COPY --from=build /app/target/backend-0.0.1-SNAPSHOT.war /app/backend.war

# Exponemos el puerto 8080 para acceder a la aplicación
EXPOSE 8080

# Comando para ejecutar la aplicación
ENTRYPOINT ["java", "-jar", "/app/backend.war"]
