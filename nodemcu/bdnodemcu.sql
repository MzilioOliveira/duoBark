-- phpMyAdmin SQL Dump
-- version 4.8.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: 23-Maio-2018 às 19:14
-- Versão do servidor: 10.1.31-MariaDB
-- PHP Version: 7.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `bdnodemcu`
--

-- --------------------------------------------------------

--
-- Estrutura da tabela `tbdados`
--

CREATE TABLE `tbdados` (
  `id` int(11) NOT NULL,
  `Ax` varchar(10) NOT NULL,
  `Ay` varchar(10) NOT NULL,
  `Az` varchar(10) NOT NULL,
  `data_hora` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `Gx` varchar(10) NOT NULL,
  `Gy` varchar(10) NOT NULL,
  `Gz` varchar(10) NOT NULL,
  `Tlm35` varchar(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `tbdados`
--

INSERT INTO `tbdados` (`id`, `Ax`, `Ay`, `Az`, `data_hora`, `Gx`, `Gy`, `Gz`, `Tlm35`) VALUES
(1, '-0.03', '0.01', '0.87', '2018-05-22 13:18:27', '-48.49', '-1.06', '1.12', '21.27'),
(2, '-0.03', '0.01', '0.89', '2018-05-22 13:18:35', '-47.15', '-1.18', '0.98', '23.53'),
(3, '-0.03', '0.01', '0.87', '2018-05-22 13:18:43', '-46.53', '-1.00', '1.18', '21.59'),
(4, '-0.03', '0.00', '0.88', '2018-05-22 13:18:51', '-46.05', '-1.24', '1.09', '21.59'),
(5, '-0.03', '0.00', '0.88', '2018-05-22 13:18:59', '-45.80', '-1.08', '1.34', '23.20'),
(6, '-0.02', '0.01', '0.88', '2018-05-22 13:19:07', '-45.56', '-1.01', '1.17', '23.20');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `tbdados`
--
ALTER TABLE `tbdados`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `tbdados`
--
ALTER TABLE `tbdados`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
