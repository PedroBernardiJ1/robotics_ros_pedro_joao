# 💻 Scripts

## 📖 Sobre este Módulo
Este módulo concentra os scripts de automação utilizados para a criação da imagem e instanciação do container

## 📁 Estrutura de Pastas

O repositório é dividido em dois arquivos:

* 📄 `build.sh`: Script responsável por construir a imagem Docker a partir do `robot.dockerfile`. Ele automatiza o comando 'docker build', encapsulando argumentos e variáveis de ambiente (como nome de usuário e UIDs) para gerar a imagem base do projeto.
* 📄 `run.sh`: Script de inicialização do ambiente. Ele automatiza o comando 'docker run', encarregando-se de montar os volumes compartilhados (vinculando a pasta ros_ws ao contêiner), habilitar o encaminhamento de interface gráfica (X11) para o RViz/simuladores e configurar a rede de comunicação do ROS.
