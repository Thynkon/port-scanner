# port-scanner
port-scanner, as it name means, it is a simple port scanner written in C using the standard libraries.

## Usage

* Scan all ports from 20 to 40

      port-scanner -a 192.168.1.10 -m 20 -M 40

* Scan all ports from 20 to 65534

      port-scanner -a 192.168.1.10 -m 20

* Scan all ports from 1 to 50

      port-scanner -a 192.168.1.10 -M 50

* Scan port 3306

      port-scanner -p 192.168.1.10:3306

* Display help

      port-scanner -h

## Installation
UNIX like

    make

## License
This programm is under the [MIT License](LICENSE).
