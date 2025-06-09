
CREATE DATABASE IF NOT EXISTS playas_oaxaca;
USE playas_oaxaca;

CREATE TABLE ubicacion (
  id_ubicacion INT AUTO_INCREMENT PRIMARY KEY,
  direccion VARCHAR(150) NOT NULL,
  municipio VARCHAR(100) NOT NULL,
  coordenadas VARCHAR(100) NOT NULL
);


CREATE TABLE caracteristicas (
  id_caracteristicas INT AUTO_INCREMENT PRIMARY KEY,
  tipo_arena VARCHAR(100),
  tipo_mar VARCHAR(100),
  oleaje VARCHAR(100),
  extension DECIMAL(5,2)
);

CREATE TABLE ambiente (
  id_ambiente INT AUTO_INCREMENT PRIMARY KEY,
  protegida TINYINT(1),
  especies_migratorias TEXT,
  nivel_contaminacion VARCHAR(100)
);

CREATE TABLE popularidad (
  id INT AUTO_INCREMENT PRIMARY KEY,
  facilidad_acceso VARCHAR(100),
  costo_entrada DECIMAL(6,2),
  visitantes_mensuales INT,
  e_popularidad INT
);

CREATE TABLE actividades (
  id_actividad INT AUTO_INCREMENT PRIMARY KEY,
  surf TINYINT(1),
  buceo TINYINT(1),
  camping TINYINT(1),
  nudismo_permitido TINYINT(1)
);

CREATE TABLE playa (
  id_playa INT AUTO_INCREMENT PRIMARY KEY,
  nombre VARCHAR(100) NOT NULL,
  id_ubicacion INT,
  id_caracteristicas INT,
  id_ambiente INT,
  id_popularidad INT,
  id_actividad INT,
  FOREIGN KEY (id_ubicacion) REFERENCES ubicacion(id_ubicacion) ON DELETE CASCADE,
  FOREIGN KEY (id_caracteristicas) REFERENCES caracteristicas(id_caracteristicas) ON DELETE CASCADE,
  FOREIGN KEY (id_ambiente) REFERENCES ambiente(id_ambiente) ON DELETE CASCADE,
  FOREIGN KEY (id_popularidad) REFERENCES popularidad(id) ON DELETE CASCADE,
  FOREIGN KEY (id_actividad) REFERENCES actividades(id_actividad) ON DELETE CASCADE
);


--INSERTAR DATOS
INSERT INTO ubicacion (direccion, municipio, coordenadas) VALUES
('San Pedro Pochutla', 'San Pedro Pochutla', ''),
('Santa María Tonameca', 'Santa María Tonameca', ''),
('Santa María Huatulco', 'Santa María Huatulco', ''),
('Puerto Ángel', 'San Pedro Pochutla', ''),
('La Bocana', 'Santa María Huatulco', ''),
('Playa Carrizalillo', 'Puerto Escondido', ''),
('Playa Manzanillo', 'Puerto Escondido', '');

INSERT INTO caracteristicas (tipo_arena, tipo_mar, oleaje, extension) VALUES
('Fina y dorada', 'Pacífico', 'Fuerte', 1.75),
('Dorada', 'Pacífico', 'Moderado', 1.30),
('Fina y blanca', 'Pacífico', 'Suave', 0.50),
('Dorada', 'Pacífico', 'Moderado', 1.20),
('Fina y blanca', 'Pacífico', 'Suave', 0.80),
('Blanca', 'Pacífico', 'Moderado', 0.30),
('Blanca', 'Pacífico', 'Moderado', 0.40);

INSERT INTO ambiente (protegida, especies_migratorias, nivel_contaminacion) VALUES
(0, 'Delfines, ballenas', 'Medio'),
(1, 'Tortugas marinas', 'Bajo'),
(1, 'Tortugas marinas, aves migratorias', 'Bajo'),
(0, 'Delfines, tortugas', 'Medio'),
(1, 'Aves migratorias', 'Bajo'),
(1, 'Ninguna', 'Alto'),
(1, 'Ninguna', 'Bajo');

INSERT INTO popularidad (facilidad_acceso, costo_entrada, visitantes_mensuales, e_popularidad) VALUES
('Media', 0.00, 15000, 7),
('Media', 0.00, 10000, 6),
('Alta', 0.00, 25000, 8),
('Media', 0.00, 12000, 6),
('Media', 0.00, 1000, 5),
('Alta', 0.00, 18000, 7),
('Alta', 0.00, 16000, 7);

INSERT INTO actividades (surf, buceo, camping, nudismo_permitido) VALUES
(1, 1, 1, 1),
(0, 1, 1, 0),
(0, 1, 1, 0),
(0, 0, 0, 0),
(1, 0, 0, 0),
(1, 1, 1, 0),
(1, 1, 1, 0);

INSERT INTO playa (nombre, id_ubicacion, id_caracteristicas, id_ambiente, id_popularidad, id_actividad) VALUES
('Zipolite', 1, 1, 1, 1, 1),
('San Agustinillo', 2, 2, 2, 2, 2),
('Bahías de Huatulco', 3, 3, 3, 3, 3),
('Puerto Ángel', 4, 4, 4, 4, 4),
('La Bocana', 5, 5, 5, 5, 5),
('Playa Carrizalillo', 6, 6, 6, 6, 6),
('Playa Manzanillo', 7, 7, 7, 7, 7);
