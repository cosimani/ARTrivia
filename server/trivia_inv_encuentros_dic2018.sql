-- phpMyAdmin SQL Dump
-- version 4.7.8
-- https://www.phpmyadmin.net/
--
-- Servidor: localhost
-- Tiempo de generación: 05-01-2019 a las 10:56:51
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
-- Estructura de tabla para la tabla `trivia_inv_encuentros`
--

CREATE TABLE `trivia_inv_encuentros` (
  `id_usuario` int(11) NOT NULL,
  `id_marcador` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcado de datos para la tabla `trivia_inv_encuentros`
--

INSERT INTO `trivia_inv_encuentros` (`id_usuario`, `id_marcador`) VALUES
(133, 2),
(133, 7),
(133, 55),
(133, 101),
(133, 103),
(133, 105),
(133, 155),
(133, 200),
(133, 252),
(133, 254),
(133, 257),
(133, 304),
(133, 306),
(133, 307),
(133, 308),
(133, 351),
(133, 355),
(133, 357),
(133, 358),
(133, 403),
(133, 406),
(133, 450),
(133, 502),
(133, 505),
(133, 550),
(133, 602),
(133, 606),
(134, 57),
(134, 101),
(134, 103),
(134, 106),
(134, 153),
(134, 200),
(134, 252),
(134, 253),
(134, 254),
(134, 307),
(134, 358),
(134, 450),
(134, 456),
(134, 607),
(135, 5),
(135, 55),
(135, 101),
(135, 103),
(135, 105),
(135, 106),
(135, 150),
(135, 155),
(135, 156),
(135, 157),
(135, 159),
(135, 200),
(135, 207),
(135, 209),
(135, 252),
(135, 253),
(135, 254),
(135, 256),
(135, 257),
(135, 259),
(135, 300),
(135, 304),
(135, 306),
(135, 307),
(135, 308),
(135, 309),
(135, 350),
(135, 351),
(135, 352),
(135, 354),
(135, 356),
(135, 359),
(135, 400),
(135, 402),
(135, 403),
(135, 404),
(135, 406),
(135, 407),
(135, 408),
(135, 450),
(135, 455),
(135, 456),
(135, 502),
(135, 505),
(135, 550),
(135, 555),
(135, 558),
(135, 602),
(135, 606),
(135, 607),
(135, 608),
(136, 57),
(136, 403),
(136, 502),
(137, 555),
(137, 607),
(138, 103),
(138, 106),
(138, 158),
(138, 250),
(138, 252),
(138, 253),
(138, 259),
(138, 300),
(138, 350),
(138, 355),
(138, 357),
(138, 404),
(138, 450),
(138, 502),
(138, 550),
(138, 607),
(139, 7),
(139, 50),
(139, 57),
(139, 101),
(139, 103),
(139, 105),
(139, 156),
(139, 200),
(139, 253),
(139, 255),
(139, 256),
(139, 259),
(139, 306),
(139, 307),
(139, 308),
(139, 350),
(139, 355),
(139, 403),
(139, 406),
(139, 450),
(139, 455),
(139, 456),
(139, 502),
(139, 505),
(139, 555),
(139, 558),
(139, 607),
(141, 57),
(141, 101),
(141, 153),
(141, 209),
(141, 253),
(141, 307),
(141, 356),
(141, 403),
(141, 406),
(141, 450),
(141, 502),
(141, 505),
(141, 550),
(141, 555),
(141, 607),
(142, 209),
(143, 7),
(143, 56),
(143, 57),
(143, 101),
(143, 103),
(143, 106),
(143, 153),
(143, 155),
(143, 200),
(143, 207),
(143, 209),
(143, 253),
(143, 254),
(143, 259),
(143, 300),
(143, 306),
(143, 307),
(143, 309),
(143, 350),
(143, 352),
(143, 355),
(143, 356),
(143, 357),
(143, 400),
(143, 402),
(143, 406),
(143, 450),
(143, 455),
(143, 456),
(143, 502),
(143, 505),
(143, 509),
(143, 550),
(143, 555),
(143, 558),
(143, 602),
(143, 605),
(143, 606),
(144, 103),
(144, 252),
(144, 300),
(144, 450),
(145, 300),
(146, 50),
(146, 56),
(146, 57),
(146, 101),
(146, 106),
(146, 150),
(146, 158),
(146, 207),
(146, 209),
(146, 307),
(146, 355),
(146, 357),
(146, 455),
(146, 502),
(146, 505),
(147, 200),
(147, 403),
(147, 406),
(147, 505),
(147, 555),
(147, 607),
(148, 5),
(148, 7),
(148, 55),
(148, 57),
(148, 101),
(148, 103),
(148, 105),
(148, 106),
(148, 108),
(148, 151),
(148, 152),
(148, 153),
(148, 154),
(148, 157),
(148, 158),
(148, 200),
(148, 209),
(148, 253),
(148, 254),
(148, 255),
(148, 259),
(148, 306),
(148, 350),
(148, 352),
(148, 353),
(148, 355),
(148, 356),
(148, 359),
(148, 403),
(148, 406),
(148, 450),
(148, 456),
(148, 502),
(148, 505),
(148, 555),
(148, 556),
(148, 602),
(148, 605),
(148, 606),
(148, 607),
(148, 608),
(149, 57),
(149, 101),
(149, 103),
(149, 153),
(149, 154),
(149, 155),
(149, 200),
(149, 209),
(149, 252),
(149, 253),
(149, 306),
(149, 307),
(149, 308),
(149, 352),
(149, 355),
(149, 400),
(149, 402),
(149, 406),
(149, 408),
(149, 450),
(149, 502),
(149, 555),
(149, 602),
(149, 607),
(150, 5),
(150, 7),
(150, 57),
(150, 101),
(150, 153),
(150, 200),
(150, 252),
(150, 307),
(150, 358),
(150, 402),
(150, 406),
(150, 450),
(150, 502),
(150, 505),
(150, 605),
(152, 6),
(152, 57),
(152, 101),
(152, 106),
(152, 158),
(152, 200),
(152, 253),
(152, 306),
(152, 307),
(152, 350),
(152, 352),
(152, 355),
(152, 357),
(152, 403),
(152, 406),
(152, 450),
(152, 502),
(152, 505),
(152, 550),
(152, 606),
(152, 607),
(153, 7),
(153, 57),
(153, 200),
(153, 306),
(153, 307),
(153, 357),
(153, 450),
(153, 555),
(153, 605),
(153, 607),
(154, 57),
(154, 103),
(154, 153),
(154, 155),
(154, 253),
(154, 307),
(154, 357),
(154, 358),
(154, 403),
(154, 406),
(154, 407),
(154, 450),
(154, 502),
(154, 607),
(156, 57),
(156, 153),
(156, 200),
(156, 307),
(156, 403),
(156, 406),
(156, 450),
(156, 605),
(157, 103),
(157, 105),
(157, 253),
(157, 300),
(157, 355),
(157, 357),
(157, 502),
(157, 602),
(157, 605),
(159, 57),
(159, 101),
(159, 106),
(159, 153),
(159, 155),
(159, 253),
(159, 300),
(159, 306),
(159, 351),
(159, 355),
(159, 450),
(160, 404),
(161, 357),
(162, 0),
(162, 2),
(162, 5),
(162, 7),
(162, 8),
(162, 57),
(162, 101),
(162, 157),
(162, 200),
(162, 209),
(162, 250),
(162, 251),
(162, 252),
(162, 253),
(162, 254),
(162, 255),
(162, 256),
(162, 257),
(162, 258),
(162, 259),
(162, 300),
(162, 306),
(162, 307),
(162, 308),
(162, 309),
(162, 350),
(162, 351),
(162, 352),
(162, 354),
(162, 355),
(162, 356),
(162, 357),
(162, 358),
(162, 359),
(162, 402),
(162, 405),
(162, 406),
(162, 450),
(162, 455),
(162, 456),
(162, 502),
(162, 505),
(162, 507),
(162, 509),
(162, 550),
(162, 555),
(162, 556),
(162, 602),
(162, 605),
(162, 606),
(162, 607),
(162, 608),
(163, 57),
(163, 101),
(163, 103),
(163, 153),
(163, 155),
(163, 157),
(163, 158),
(163, 200),
(163, 209),
(163, 252),
(163, 253),
(163, 300),
(163, 306),
(163, 307),
(163, 308),
(163, 350),
(163, 355),
(163, 356),
(163, 357),
(163, 403),
(163, 406),
(163, 450),
(163, 455),
(163, 502),
(163, 505),
(163, 558),
(163, 602),
(163, 605),
(163, 607),
(165, 101),
(165, 153),
(165, 209),
(165, 304),
(165, 403),
(165, 406),
(166, 57),
(166, 101),
(166, 103),
(166, 105),
(166, 108),
(166, 200),
(168, 6),
(169, 5),
(169, 6),
(169, 7),
(169, 8),
(169, 57),
(169, 101),
(169, 103),
(169, 105),
(169, 106),
(169, 155),
(169, 157),
(169, 158),
(169, 200),
(169, 207),
(169, 208),
(169, 209),
(169, 252),
(169, 253),
(169, 254),
(169, 255),
(169, 256),
(169, 258),
(169, 259),
(169, 300),
(169, 306),
(169, 307),
(169, 309),
(169, 350),
(169, 351),
(169, 352),
(169, 355),
(169, 356),
(169, 357),
(169, 358),
(169, 400),
(169, 402),
(169, 403),
(169, 404),
(169, 406),
(169, 450),
(169, 456),
(169, 502),
(169, 505),
(169, 550),
(169, 555),
(169, 558),
(169, 602),
(169, 605),
(169, 606),
(169, 607),
(169, 608),
(172, 103),
(172, 106),
(172, 157),
(172, 403),
(172, 450),
(172, 505),
(172, 605),
(172, 607),
(175, 57),
(175, 252),
(175, 403),
(175, 502),
(175, 605),
(175, 607),
(176, 252),
(176, 300),
(176, 307),
(176, 602),
(176, 607),
(177, 57),
(177, 607),
(178, 103),
(178, 158),
(178, 253),
(178, 259),
(178, 306),
(178, 307),
(178, 402),
(178, 403),
(178, 406),
(178, 450),
(178, 456),
(178, 502),
(178, 558),
(179, 101),
(179, 157),
(179, 307),
(179, 400),
(179, 406),
(179, 450),
(179, 509),
(179, 607),
(180, 101),
(180, 259),
(180, 307),
(180, 402),
(180, 450),
(180, 607),
(184, 6),
(184, 103),
(184, 157),
(184, 252),
(184, 253),
(184, 306),
(184, 350),
(184, 357),
(184, 358),
(184, 403),
(184, 450),
(185, 50),
(185, 55),
(185, 56),
(185, 101),
(185, 103),
(185, 108),
(185, 150),
(185, 152),
(185, 157),
(185, 158),
(185, 159),
(185, 200),
(185, 209),
(185, 250),
(185, 252),
(185, 253),
(185, 254),
(185, 300),
(185, 307),
(185, 308),
(185, 309),
(185, 350),
(185, 355),
(185, 357),
(185, 403),
(185, 406),
(185, 407),
(185, 409),
(185, 450),
(185, 502),
(185, 505),
(185, 509),
(185, 550),
(185, 556),
(185, 558),
(185, 602),
(185, 605),
(185, 606),
(185, 607),
(186, 101),
(186, 103),
(186, 106),
(186, 153),
(186, 155),
(186, 157),
(186, 158),
(186, 252),
(186, 253),
(186, 259),
(186, 306),
(186, 307),
(186, 308),
(186, 353),
(186, 355),
(186, 356),
(186, 357),
(186, 402),
(186, 403),
(186, 406),
(186, 407),
(186, 450),
(186, 502),
(186, 505),
(186, 550),
(186, 558),
(186, 602),
(186, 607),
(188, 5),
(188, 101),
(188, 103),
(188, 106),
(188, 150),
(188, 157),
(188, 158),
(188, 200),
(188, 209),
(188, 252),
(188, 253),
(188, 254),
(188, 306),
(188, 357),
(188, 403),
(188, 450),
(188, 502),
(188, 550),
(188, 555),
(188, 558),
(188, 606),
(189, 101),
(189, 103),
(189, 253),
(189, 307),
(189, 450),
(189, 555),
(189, 607),
(193, 101),
(196, 103),
(196, 304),
(197, 153),
(197, 155),
(197, 352),
(197, 406),
(197, 450),
(198, 153),
(198, 155),
(198, 358),
(198, 402),
(198, 450),
(199, 153),
(199, 350),
(199, 406),
(199, 450),
(200, 153),
(200, 350),
(200, 358),
(200, 406),
(200, 450),
(201, 251),
(201, 300),
(201, 406),
(201, 450),
(202, 101),
(202, 153),
(202, 155),
(202, 157),
(202, 200),
(202, 253),
(202, 257),
(202, 306),
(202, 355),
(202, 356),
(202, 405),
(202, 406),
(202, 450),
(202, 456),
(202, 550),
(202, 602);

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `trivia_inv_encuentros`
--
ALTER TABLE `trivia_inv_encuentros`
  ADD PRIMARY KEY (`id_usuario`,`id_marcador`);

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla `trivia_inv_encuentros`
--
ALTER TABLE `trivia_inv_encuentros`
  ADD CONSTRAINT `fk__trivia_inv_encuentros__trivia_inv_usuarios__1__end` FOREIGN KEY (`id_usuario`) REFERENCES `trivia_inv_usuarios` (`id_usuario`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
