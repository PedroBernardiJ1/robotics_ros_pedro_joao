# ⚙️ Config

## 📖 Sobre este Módulo
Este módulo concentra arquivos de configuração injetados no contêiner (ex: atalhos e aliases no `bashrc`).

## 📁 Estrutura de Pastas

O repositório é dividido em três áreas principais:

* 📄 `bashrc`: Arquivo de configuração do terminal injetado no contêiner. Ele customiza a experiência de uso no terminal interno, garantindo que o ROS seja ativado automaticamente (source /opt/ros/...) sempre que uma nova aba for aberta, além de habilitar o autocompletar e personalizar as cores do terminal.
* 📄 `tools.sh`: Arquivo complementar de utilitários. Geralmente integrado ao bashrc, ele armazena atalhos (aliases) customizados e funções úteis criadas por você para agilizar o fluxo de trabalho (por exemplo, comandos curtos para compilar pacotes rapidamente com o colcon ou limpar o workspace).


