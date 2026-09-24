# 🤖 REPOSITÓRIO ROS/DOCKER DO SONHO

## 📖 Sobre o Repositório
Este repositório consolida as atividades, listas e projetos de capacitação em sistemas autônomos. Toda a infraestrutura foi desenhada para isolar as dependências utilizando **Docker**, garantindo que o ambiente de compilação do **ROS 2** seja limpo, reprodutível e independente da máquina host.

---

## 📁 Estrutura do Repositório

Abaixo estão os links diretos para as documentações e códigos elaborados de cada subtema, organizados por módulo:

### ⚙️ [Módulo: Docker](./Docker)
Este módulo contém toda infraestrutura do ambiente virtual.

* 📄 [robot.dockerfile](./docker/robot.dockerfile)
* 📁 [scripts](./docker/scripts)
* 📁 [config]((./docker/config)

### 🤖 [Módulo: ROS](./ros_ws)
Este módulo contém todos os workspaces de desenvolvimento do ROS.

* 📁 [src](./ros_ws/src):

---

## 📌 Projetos e Entregas
Abaixo estão os links rápidos para as documentações específicas de cada módulo e subprojeto desenvolvido.

### Módulo de Infraestrutura (Docker)
* 📁 [Lista 1 - TEMA LISTA 1](./docker/???)

### Módulo de Desenvolvimento (ROS)
Conforme os pacotes forem criados em ros_ws/src, seus respectivos links serão adicionados abaixo:

* 📁 [aula_3_python_nodes](./ros_ws/src/aula_3_python_nodes)
* 📁 [aula_3_publisher_cpp](./ros_ws/src/aula_3_publisher_cpp)
* 📁 [aula_3_subscriber_cpp](./ros_ws/src/aula_3_subscriber_cpp)
* 📁 [tutoria_node_1](./ros_ws/src/tutoria_node_1)
* 📁 [tutoria_node_2](./ros_ws/src/tutoria_node_2)
* 📁 [tutoria2_node_1](./ros_ws/src/tutoria2_node_1)
* 📁 [tutoria2_node_2](./ros_ws/src/tutoria2_node_2)
* 📁 [projeto1_publisher_subscriber](./ros_ws/src/projeto1_publisher_subscriber)
* 📁 [tutoria3_node_1](./ros_ws/src/tutoria3_node_1)
* 📁 [tutoria3_node_2](./ros_ws/src/tutoria3_node_2)
* 📁 [aula_6_robot_interfaces](./ros_ws/src/aula_6_robot_interfaces)
* 📁 [tutoria4_custom_interfaces](./ros_ws/src/tutoria4_custom_interfaces)
* 📁 [tutoria4_node_1](./ros_ws/src/tutoria4_node_1)
* 📁 [tutoria4_node_2](./ros_ws/src/tutoria4_node_2)
* 📁 [tutoria4_bringup](./ros_ws/src/tutoria4_bringup)

---

## 🚀 Como Inicializar o Ambiente

Para compilar ou executar qualquer código deste repositório, você deve primeiro iniciar o contêiner Docker. 

1. **Construir a imagem (Build):**
   Execute este comando na raiz do repositório caso seja a sua primeira vez rodando o projeto ou caso o `robot.dockerfile` tenha sido alterado:
   
   ```bash
   bash docker/scripts/build.sh
   ```
   
2. **Rodar o contêiner (Run):**
   Inicie o ambiente de desenvolvimento isolado. Este script já cuida de fazer o bind mount (vincular) a pasta ros_ws para dentro do contêiner:
   
   ```bash
   bash docker/scripts/run.sh
   ```

---
*Desenvolvido e documentado por Pedro Bernardi João.*
