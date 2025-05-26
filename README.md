# basededatos
CREATE DATABASE IF NOT EXISTS hospital;
USE hospital;

CREATE TABLE ubicacion (
    id_ubicacion INT AUTO_INCREMENT PRIMARY KEY,
    direccion VARCHAR(200) NOT NULL,
    municipio VARCHAR(45) NOT NULL,
    coordenadas VARCHAR(45)
);

CREATE TABLE tipo (
    id_tipo INT AUTO_INCREMENT PRIMARY KEY,
    tipo VARCHAR(45) NOT NULL
);

CREATE TABLE medicamento (
    id_medicamento INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    precio DECIMAL(10,2) NOT NULL
);

CREATE TABLE aparato (
    id_aparato INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL
);

CREATE TABLE hospital (
    id_hospital INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    ubicacion_id INT NOT NULL,
    tipo_id INT NOT NULL,
    FOREIGN KEY (ubicacion_id) REFERENCES ubicacion(id_ubicacion),
    FOREIGN KEY (tipo_id) REFERENCES tipo(id_tipo)
);

CREATE TABLE paciente (
    id_paciente INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    hospital_id INT NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES hospital(id_hospital)
);

CREATE TABLE personal (
    id_personal INT AUTO_INCREMENT PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    rol VARCHAR(45) NOT NULL,
    hospital_id INT NOT NULL,
    FOREIGN KEY (hospital_id) REFERENCES hospital(id_hospital)
);

CREATE TABLE inventario_medicamentos (
    hospital_id INT NOT NULL,
    medicamento_id INT NOT NULL,
    cantidad_stock INT NOT NULL,
    precio_unitario DECIMAL(10,2) NOT NULL,
    PRIMARY KEY (hospital_id, medicamento_id),
    FOREIGN KEY (hospital_id) REFERENCES hospital(id_hospital),
    FOREIGN KEY (medicamento_id) REFERENCES medicamento(id_medicamento)
);

CREATE TABLE hospital_has_aparato (
    hospital_id INT NOT NULL,
    aparato_id INT NOT NULL,
    cantidad VARCHAR(45),
    estado VARCHAR(45),
    fecha_adquisicion DATE,
    PRIMARY KEY (hospital_id, aparato_id),
    FOREIGN KEY (hospital_id) REFERENCES hospital(id_hospital),
    FOREIGN KEY (aparato_id) REFERENCES aparato(id_aparato)
);

INSERT INTO ubicacion (direccion, municipio, coordenadas) VALUES
    ('Av. Reforma 123', 'Oaxaca', '17.0603,-96.7256'),
    ('Calle 5 de Mayo 456', 'Zaachila', '17.0436,-96.6461'),
    ('C. Independencia 789', 'Tlacolula', '16.9265,-96.4682');

INSERT INTO tipo (tipo) VALUES
    ('General'),
    ('Pediátrico'),
    ('Urgencias');

INSERT INTO medicamento (nombre, precio) VALUES
    ('Paracetamol', 25.00),
    ('Ibuprofeno', 30.50),
    ('Amoxicilina', 45.75);

INSERT INTO aparato (nombre) VALUES
    ('Tomógrafo'),
    ('Resonancia Magnética'),
    ('Ecógrafo');

INSERT INTO hospital (nombre, ubicacion_id, tipo_id) VALUES
    ('Hospital Central', 1, 1),
    ('Hospital Pediátrico', 2, 2),
    ('Clínica Urgencias', 3, 3);

INSERT INTO paciente (nombre, hospital_id) VALUES
    ('Juan Pérez', 1),
    ('María López', 2),
    ('Carlos Hernández', 1);

INSERT INTO personal (nombre, rol, hospital_id) VALUES
    ('Dr. Manuel Sánchez', 'Director', 1),
    ('Enfermera Ana Gómez', 'Enfermera', 1),
    ('Dr. Luis Martínez', 'Médico Urgencias', 3);

INSERT INTO inventario_medicamentos (hospital_id, medicamento_id, cantidad_stock, precio_unitario) VALUES
    (1, 1, 200, 24.50),
    (1, 2, 150, 29.75),
    (2, 3, 100, 44.00);

INSERT INTO hospital_has_aparato (hospital_id, aparato_id, cantidad, estado, fecha_adquisicion) VALUES
    (1, 1, '1', 'Operativo', '2022-01-10'),
    (1, 3, '2', 'Operativo', '2021-07-20'),
    (3, 2, '1', 'Mantenimiento', '2023-03-05');
