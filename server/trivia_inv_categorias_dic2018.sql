-- phpMyAdmin SQL Dump
-- version 4.7.8
-- https://www.phpmyadmin.net/
--
-- Servidor: localhost
-- Tiempo de generación: 05-01-2019 a las 10:56:43
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
-- Estructura de tabla para la tabla `trivia_inv_categorias`
--

CREATE TABLE `trivia_inv_categorias` (
  `id_categoria` int(11) NOT NULL,
  `nombre_categoria` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcado de datos para la tabla `trivia_inv_categorias`
--

INSERT INTO `trivia_inv_categorias` (`id_categoria`, `nombre_categoria`) VALUES
(8, 'Administración y negocios'),
(4, 'Arte'),
(3, 'Ciencias'),
(9, 'Contabilidad'),
(1, 'Cultura general'),
(5, 'Deportes'),
(12, 'Educación cívica'),
(2, 'Entretenimiento'),
(6, 'Geografía'),
(7, 'Informática'),
(11, 'Medioambiente - Higiene y seguridad'),
(10, 'Salud');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `trivia_inv_categorias`
--
ALTER TABLE `trivia_inv_categorias`
  ADD PRIMARY KEY (`id_categoria`),
  ADD UNIQUE KEY `uq__trivia_inv_categorias__1__end` (`nombre_categoria`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
