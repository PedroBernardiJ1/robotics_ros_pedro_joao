# 📂 Diretório de Pacotes ROS (`ros_ws/src`)

Este diretório contém todo o código-fonte e os pacotes ROS 2 desenvolvidos para as entregas de capacitação do Projeto Sonho. Ele atua como a área de trabalho central (workspace) para a criação e compilação de nós utilizando o sistema `colcon`.

---

## 🏗️ Mapa de Projetos e Pacotes

Dentro desta pasta, os códigos estão divididos por subprojetos ou listas de tarefas. Cada pasta listada abaixo agrupa pacotes com contextos semelhantes:

*   📁 [aula_3_python_nodes](./aula_3_python_nodes): Nós 'publisher' e 'subscriber' implementados na Aula 3 de ROS, em Python;

*   📁 [aula_3_publisher_cpp](./aula_3_publisher_cpp): Nó 'publisher_cpp' implementado na Aula 3 de ROS, em C++;

*   📁 [aula_3_subscriber_cpp](./aula_3_subscriber_cpp): Nó 'subscriber_cpp' implementado na Aula 3 de ROS, em C++;

*   📁 [tutoria_node_1](./tutoria_node_1): Nó publisher ("/publisher_node_area") implementado na Tutoria Referente à Aula 3 para calcular a área de um círculo, em C++;

*   📁 [tutoria_node_2](./tutoria_node_2): Nó subscriber ("/publisher_node_area") e publisher ("/publisher_volume") implementado na Tutoria Referente à Aula 3 para receber a área calculada do círculo, definir uma altura e calcular o volume de um cilindro, em Python;

*   📁 [tutoria2_node_1](./tutoria2_node_1): Nó subscriber ("/point_in") e publisher ("/distance" e "/orientation") implementado na Tutoria Referente à Aula 4 para receber um ponto (x, y, z) e, a partir dele, calcular e publicar a distância da origem e as orientações theta e phi, em C++;

*   📁 [tutoria2_node_2](./tutoria2_node_2): Nó subscriber ("/distance" e "/orientation") e publisher ("/point_out") implementado na Tutoria Referente à Aula 4 para receber a distância e os ângulos theta e phi e, a partir deles, converter novamente para o ponto original dado ("/point_in"), em Python;

*   📁 [projeto1_publisher_subscriber](./projeto1_publisher_subscriber): Nó subscriber ("/goal" e "/robot_position") e publisher ("/cmd_vel") que recebe um ponto de destino (Vector3) e uma pose e orientação atuais (Pose) para retornar comandos de velocidade linear e angular até chegar ao destino;

*   📁 [tutoria3_node_1](./tutoria3_node_1): Nó publisher ("/imu" e "/odom") que declara a velocidade angular do IMU em X, Y e Z e a covariância em Z da Odometria, em C++; 

*   📁 [aula_6_robot_interfaces](./aula_6_robot_interfaces): Nó de configuração das mensagens customizadas "Obstacle" e "Obstacles", implementado na Aula 6 de ROS;

*   📁 [tutoria4_custom_interfaces](./tutoria4_custom_interfaces): Nó de configuração das mensagens customizadas da Tutoria 4, "Diagnostico" e "Velocidade";

*   📁 [tutoria4_node_1](./tutoria4_node_1): Nó publisher ("/velocidade") da Tutoria 4, em Python;

*   📁 [tutoria4_node_2](./tutoria4_node_2): Nó subscriber ("/velocidade") e publisher ("/diagnostico") da Tutoria 4, em C++;

*   📁 [tutoria4_bringup](./tutoria4_bringup): LaunchFile dos dois nós da Tutoria 4;

*   📁 [system_monitor_interfaces](./system_monitor_interfaces): Nó de configuração das mensagens customizadas do Hackaton 1 de ROS/Python, "Cpu", "Disco", "Ram" e "Monitoramento";

*   📁 [system_monitor](./system_monitor): Nó publisher ("/hardware_status") do Hackaton 1 de ROS/Python, em Python;

*   📁 [aula_7_custom_interfaces](./aula_7_custom_interfaces): Nó de criação dos Serviços da Aula 7 ("AddTwoInts.srv" e "VectorDistance.srv")

*   📁 [aula_7_mathematics_operations](./aula_7_mathematics_operations): Contém a implementação do serviço "AddTwoInts" a partir do nó cliente ("add_ints_cliente_node") e do nó servidor ("add_ints_server_node"), em C++;

*   📁 [aula_7_physics_operations](./ros_ws/src/aula_7_physics_operations): Contém a implementação do serviço "VectorDistance" a partir do nó cliente ("vector_distance_cliente_node") e do nó servidor ("vector_distance_server_node"), em Python;

---

## 🛠️ Comandos Frequentes (Cheatsheet)

Como esta pasta é o centro de desenvolvimento, aqui estão os comandos essenciais para o fluxo de trabalho. 

**⚠️ Atenção:** Todos os comandos abaixo devem ser executados a partir da raiz do workspace (`~/ros_ws`) e **sempre por dentro do contêiner Docker**.

*   **Compilar todos os pacotes do repositório:**
    ```bash
    colcon build --symlink-install
    ```
    
    *(Dica: A flag `--symlink-install` cria links simbólicos para scripts Python. Isso significa que você pode editar o código Python e rodar novamente sem precisar recompilar tudo de novo!)*

*   **Compilar apenas um pacote específico (economiza muito tempo):**

    ```bash
    colcon build --packages-select nome_do_pacote
    ```

*   **Atualizar as variáveis de ambiente (Sourcing):**
    Após criar ou compilar novos nós, é necessário "avisar" o terminal sobre a existência deles:
    
    ```bash
    source install/setup.bash
    ```

*   **Rodar um nó específico:**

    ```bash
    ros2 run nome_do_pacote nome_do_executavel
    ```


