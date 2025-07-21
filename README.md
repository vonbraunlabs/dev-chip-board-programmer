# DevChipBoard Programmer

Este é um fork do [PICO-DIRTYJTAG](github.com/phdussud/pico-dirtyJtag), que é um fork do [DirtyJtag](https://github.com/jeanthom/DirtyJTAG), adaptado para funcionar com a RP2040 embarcada na DevChipBoard v3.

Este projeto transforma a RP2040 num cabo JTAG, permitindo gravar uma FPGA conectada via barramento JTAG. Adicionalmente possui um adaptador serial incluso.

## 🛠️ Configuração Rápida

### Pré-requisitos

-[RaspberryPi Pico SDK](https://github.com/raspberrypi/pico-sdk)


### Compilação do firmware

1. **Instale a PICO SDK conforme as instruções do repositório oficial**

[Instruções oficiais do Raspberry Pi PICO SDK](https://github.com/raspberrypi/pico-sdk)

2. **Crie o diretório para a build e compile o projeto**
   ```bash
   mkdir -p build
   cd build
   cmake ..
   make
   ```
3. **Conecte a DevChipBoard no modo de gravação de firmware da RP2040**

Se algum firmware já foi gravado previamente na RP2040, é necessário conectar o pino 1 (o pino quadrado) do conector H2 ao GND antes de conectar a DevChipBoard ao computador. Se é a primeira vez, apenas conecte a placa ao computador que a RP2040 irá iniciar no modo gravação automaticamente.

Verifique qual caminho foi atribuída a RP2040 conectada utilizando o comando:

```bash
sudo fdisk -l
```

4. **Com a DevChipBoard conectada e a RP2040 no modo gravação, grave o firmware compilado**

**ATENÇÃO**: Se algum firmware já foi gravado previamente na RP2040, é necessário conectar o pino 1 (o pino quadrado) do conector H2 ao GND antes de conectar a DevChipBoard ao computador.

   ```bash
   mkdir ~/mnt
   mount /dev/sdX ~/mnt
   cp chipProgrammer.uf2 ~/mnt
   umount ~/mnt
   
   ```
