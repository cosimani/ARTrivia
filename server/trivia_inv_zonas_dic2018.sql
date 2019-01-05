-- phpMyAdmin SQL Dump
-- version 4.7.8
-- https://www.phpmyadmin.net/
--
-- Servidor: localhost
-- Tiempo de generación: 05-01-2019 a las 10:56:27
-- Versión del servidor: 5.5.41-MariaDB
-- Versión de PHP: 5.5.38

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `ceos1720_vayradb`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `trivia_inv_zonas`
--

CREATE TABLE `trivia_inv_zonas` (
  `id_zona` int(11) NOT NULL,
  `nombre_zona` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcado de datos para la tabla `trivia_inv_zonas`
--

INSERT INTO `trivia_inv_zonas` (`id_zona`, `nombre_zona`) VALUES
(10, 'Biblioteca'),
(3, 'Cantina'),
(13, 'Castillo UBP tour'),
(1, 'Estacionamiento'),
(7, 'Estudio TV'),
(2, 'Fuente del jardín'),
(11, 'Galería del rectorado'),
(6, 'Laboratorios'),
(8, 'Pasillos aulas 200 y 300'),
(4, 'Pasillos aulas 400'),
(9, 'Pasillos aulas 600'),
(5, 'Patio de tutorías'),
(12, 'Residencias');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `trivia_inv_zonas`
--
ALTER TABLE `trivia_inv_zonas`
  ADD PRIMARY KEY (`id_zona`),
  ADD UNIQUE KEY `uq__trivia_inv_zonas__1__end` (`nombre_zona`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
