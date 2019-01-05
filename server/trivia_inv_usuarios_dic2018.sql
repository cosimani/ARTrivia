-- phpMyAdmin SQL Dump
-- version 4.7.8
-- https://www.phpmyadmin.net/
--
-- Servidor: localhost
-- Tiempo de generación: 05-01-2019 a las 10:57:00
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
-- Estructura de tabla para la tabla `trivia_inv_usuarios`
--

CREATE TABLE `trivia_inv_usuarios` (
  `id_usuario` int(11) NOT NULL,
  `nombre_usuario` varchar(100) NOT NULL,
  `clave_usuario` varchar(100) NOT NULL,
  `mail_usuario` varchar(100) NOT NULL,
  `access_token` varchar(100) NOT NULL,
  `id_facebook` varchar(100) NOT NULL,
  `nombre_facebook` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcado de datos para la tabla `trivia_inv_usuarios`
--

INSERT INTO `trivia_inv_usuarios` (`id_usuario`, `nombre_usuario`, `clave_usuario`, `mail_usuario`, `access_token`, `id_facebook`, `nombre_facebook`) VALUES
(132, 'César Osimani', '1234', '26413920', 'No tiene', '26413920', 'No tiene'),
(133, 'agustin benvenuto', '1234', '42232035', 'No tiene', '42232035', 'No tiene'),
(134, 'tania roldan', '22328240', '44371278', 'No tiene', '44371278', 'No tiene'),
(135, 'milagros guadalupe zeballos buchieri', '123456', '44371300', 'No tiene', '44371300', 'No tiene'),
(136, 'milagros grassi', '12345678', '44740164', 'No tiene', '44740164', 'No tiene'),
(137, 'navarro hector fernando', 'calamardo2021', '42550122', 'No tiene', '42550122', 'No tiene'),
(138, 'jorge nahuel mereles', 'jorgenahuel01', '41718523', 'No tiene', '41718523', 'No tiene'),
(139, 'calderon julieta', '45246275', '45246275', 'No tiene', '45246275', 'No tiene'),
(140, 'valerie barrios', '44371210', '44371210', 'No tiene', '44371210', 'No tiene'),
(141, 'nahuel beeguer', 'jaja2323', '41975549', 'No tiene', '41975549', 'No tiene'),
(142, 'federico leonardo andres stagnaro', 'fede123', '42246470', 'No tiene', '42246470', 'No tiene'),
(143, 'nahuel yavarone', 'yavarone99', '41993238', 'No tiene', '41993238', 'No tiene'),
(144, 'ramiro aparicio', 'holahola', '41820694', 'No tiene', '41820694', 'No tiene'),
(145, 'alejandro matias loza', '12345678', '43370316', 'No tiene', '43370316', 'No tiene'),
(146, 'moreno maximiliano rodrigo', '3512117087', '42896105', 'No tiene', '42896105', 'No tiene'),
(147, 'natalia delgado', 'nati2001', '43144071', 'No tiene', '43144071', 'No tiene'),
(148, 'ezequiel matÃ­as brisuela', 'codigoqr', '46221628', 'No tiene', '46221628', 'No tiene'),
(149, 'ramiro martÃ­n poyo', 'Ramiro14', '43144027', 'No tiene', '43144027', 'No tiene'),
(150, 'luduena martÃ­n', 'maximo14', '43449192', 'No tiene', '43449192', 'No tiene'),
(151, 'lisi gonzalez', 'vikinga', '43231709', 'No tiene', '43231709', 'No tiene'),
(152, 'lautaro heredia carranza', 'Lautipk', '43450176', 'No tiene', '43450176', 'No tiene'),
(153, 'sanchez santiago', 'rallywrc', '43368469', 'No tiene', '43368469', 'No tiene'),
(154, 'mauro angiolini', '43231788', '43231788', 'No tiene', '43231788', 'No tiene'),
(155, 'gustavo andres peron', 'gusty123', '43450121', 'No tiene', '43450121', 'No tiene'),
(156, 'gaston barberis', '42977270', '42977270', 'No tiene', '42977270', 'No tiene'),
(157, 'marÃ­a guadalupe salvatierra', 'guaada', '42317011', 'No tiene', '42317011', 'No tiene'),
(158, 'juan pablo morelli', '306077014', '44638329', 'No tiene', '44638329', 'No tiene'),
(159, 'bianca valente araoz', 'bianca', '43272529', 'No tiene', '43272529', 'No tiene'),
(160, 'ulloa bahamonde jose luis', 'Yesi2018', '33621374', 'No tiene', '33621374', 'No tiene'),
(161, 'maria luisina rodriguez', 'LuisinaRodriguez123', '43143387', 'No tiene', '43143387', 'No tiene'),
(162, 'petrei maximiliano agustin', 'maxissoniet1234', '42642710', 'No tiene', '42642710', 'No tiene'),
(163, 'milena abril munoz sierra', 'mamapapa1075', '43272769', 'No tiene', '43272769', 'No tiene'),
(164, 'facundo cordoba', '1234567890', '42109449', 'No tiene', '42109449', 'No tiene'),
(165, 'leandro somazzi', 'fabrii', '44194917', 'No tiene', '44194917', 'No tiene'),
(166, 'walter carlos gorosito', 'gorwal', '22954130', 'No tiene', '22954130', 'No tiene'),
(167, 'enzo peyon', 'yanoaguanto', '42915157', 'No tiene', '42915157', 'No tiene'),
(168, 'zalazar rodrigo', '284348007', '42943904', 'No tiene', '42943904', 'No tiene'),
(169, 'lautaro manuel gill', 'gearrex', '43672958', 'No tiene', '43672958', 'No tiene'),
(170, 'german gozdziewski', 'Emilia', '23476714', 'No tiene', '23476714', 'No tiene'),
(171, 'ayelen de los angeles chabes', '284348006', '43469783', 'No tiene', '43469783', 'No tiene'),
(172, 'eliseo chiappone', 'chel1512', '42855502', 'No tiene', '42855502', 'No tiene'),
(173, 'lautaro milovich', '308172002', '42301892', 'No tiene', '42301892', 'No tiene'),
(174, 'enzo javier bejarano', '308172004', '43535781', 'No tiene', '43535781', 'No tiene'),
(175, 'ariel lizarraga', '43271153', '43271153', 'No tiene', '43271153', 'No tiene'),
(176, 'marcos rodrÃ­guez', 'eurocase123', '43273408', 'No tiene', '43273408', 'No tiene'),
(177, 'gustavo avendano', '283346012', '44015819', 'No tiene', '44015819', 'No tiene'),
(178, 'juan emanuel de bock', 'skere20', '43639561', 'No tiene', '43639561', 'No tiene'),
(179, 'raul hernandez', 'riptide1', '43640568', 'No tiene', '43640568', 'No tiene'),
(180, 'franco calani torrez', '283346014', '43951652', 'No tiene', '43951652', 'No tiene'),
(181, 'danilo gonzalo vargas', 'tuhermana12', '43951733', 'No tiene', '43951733', 'No tiene'),
(182, 'leandro gabriel nunez', 'holadenueva2018', '42815512', 'No tiene', '42815512', 'No tiene'),
(183, 'guillermo gabriel enrique', 'gabi4823714', '42642232', 'No tiene', '42642232', 'No tiene'),
(184, 'uriarte joel', 'akjjyglc1', '45085693', 'No tiene', '45085693', 'No tiene'),
(185, 'juan martin', 'juanmajuanma', '43372743', 'No tiene', '43372743', 'No tiene'),
(186, 'alpha centuri', 'rodri159', '43762875', 'No tiene', '43762875', 'No tiene'),
(187, 'lucas villalba', 'tuhermana12', '41978473', 'No tiene', '41978473', 'No tiene'),
(188, 'colqui anahi inocencia', '283346004', '43375206', 'No tiene', '43375206', 'No tiene'),
(189, 'mikaela', 'mikalook', '44672966', 'No tiene', '44672966', 'No tiene'),
(190, 'bautista schneeberger', 'yenes395sch1', '4210018p', 'No tiene', '4210018p', 'No tiene'),
(191, 'elian paredes', 'electro12', '42595080', 'No tiene', '42595080', 'No tiene'),
(193, 'leal', '3301384a', '42854295', 'No tiene', '42854295', 'No tiene'),
(194, 'franco emiliano quiroga', 'boca12bb', '43231782', 'No tiene', '43231782', 'No tiene'),
(195, 'boisseau carolina', '42977976', '42977976', 'No tiene', '42977976', 'No tiene'),
(196, 'fabian salerno', 'fabiansalerno', '42337731', 'No tiene', '42337731', 'No tiene'),
(197, 'felipe', 'yago2024', '42383949', 'No tiene', '42383949', 'No tiene'),
(198, 'juan quintero', 'yagoyagoyago', '41524130', 'No tiene', '41524130', 'No tiene'),
(199, 'julian luna', 'hola1234', '42053048', 'No tiene', '42053048', 'No tiene'),
(200, 'nicolas bazan', 'yago2023', '42385215', 'No tiene', '42385215', 'No tiene'),
(201, 'martin montsne', 'Powerder1', '42784176', 'No tiene', '42784176', 'No tiene'),
(202, 'uran tomas', 'pili1512', '43695137', 'No tiene', '43695137', 'No tiene');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `trivia_inv_usuarios`
--
ALTER TABLE `trivia_inv_usuarios`
  ADD PRIMARY KEY (`id_usuario`),
  ADD UNIQUE KEY `uq__trivia_inv_usuarios__1__end` (`mail_usuario`),
  ADD UNIQUE KEY `uq__trivia_inv_usuarios__2__end` (`id_facebook`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `trivia_inv_usuarios`
--
ALTER TABLE `trivia_inv_usuarios`
  MODIFY `id_usuario` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=203;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
