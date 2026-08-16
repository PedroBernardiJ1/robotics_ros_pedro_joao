# 📂 Diretório de Pacotes ROS (`ros_ws/src`)

Este diretório contém todo o código-fonte e os pacotes ROS 2 desenvolvidos para os sistemas autônomos e para as entregas de capacitação do Projeto Sonho. Ele atua como a área de trabalho central (workspace) para a criação e compilação de nós utilizando o sistema `colcon`.

---

## 🏗️ Mapa de Projetos e Pacotes

Dentro desta pasta, os códigos estão divididos por subprojetos ou listas de tarefas. Cada pasta listada abaixo agrupa pacotes com contextos semelhantes:

*   📁 [sub_pasta_de_projeto1](./sub_pasta_de_projeto_1): (SUBSTITUIR "Módulo de integração de sensores", por exemplo)
    *   📦 [pacote_entrega_1](./pacote_entrega_1): (SUBSTITUIR "Nó responsável por ler os dados do laser scan", por exemplo)

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


