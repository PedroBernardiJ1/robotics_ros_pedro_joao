# Etapa 1: Imagem base com ROS Jazzy
FROM ros:jazzy

# Evita que qualquer instalação peça uma interação com o usuário
ENV DEBIAN_FRONTEND=noninteractive

# Etapa 2: Pacotes de sistema (com terminal ZSH)
RUN apt-get update && apt-get install -y --no-install-recommends \
    sudo \
    git \
    gedit \
    nano \
    jq \
    gdb \
    build-essential \
    tree \
    python3 \
    python3-pip \
    python3-colcon-common-extensions \
    python3-vcstool \
    python3-rosdep \
    python3-argcomplete \
    zsh \
    curl \
  && rm -rf /var/lib/apt/lists/*

# Etapa 3: Pacotes ROS
RUN apt-get update && apt-get install -y --no-install-recommends \
    ros-jazzy-xacro \
    ros-jazzy-robot-state-publisher \
    ros-jazzy-joint-state-publisher \
    ros-jazzy-rviz2 \
    ros-jazzy-rqt* \
    ros-jazzy-rqt-common-plugins \
    ros-jazzy-launch \
    ros-jazzy-launch-ros \
    ros-jazzy-ros-gz \
    ros-jazzy-gz-ros2-control \
    ros-jazzy-ros2-control \
    ros-jazzy-ros2-controllers \
    ros-jazzy-controller-manager \
    ros-jazzy-joint-state-broadcaster \
    ros-jazzy-tf-transformations \
    && rm -rf /var/lib/apt/lists/*

# Inicializa o rosdep (precisa ser root). A imagem base ros:jazzy já vem com isso pronto, então só rodamos "rosdep init" se o arquivo ainda não existir.
RUN [ -e /etc/ros/rosdep/sources.list.d/20-default.list ] || rosdep init

# Etapa 4: Configuração do ambiente de usuário
ARG USERNAME=host
ARG USER_UID=1000
ARG USER_GID=1000

RUN set -eux; \
    # renomeia grupo 1000 para 'host' (se ainda não tiver esse nome)
    if [ "$(getent group ${USER_GID} | cut -d: -f1)" != "${USERNAME}" ]; then \
        groupmod -n "${USERNAME}" "$(getent group ${USER_GID} | cut -d: -f1)"; \
    fi; \
    # renomeia user 1000 para 'host' e move a home para /home/host
    if [ "$(getent passwd ${USER_UID} | cut -d: -f1)" != "${USERNAME}" ]; then \
        usermod -l "${USERNAME}" -d "/home/${USERNAME}" -m "$(getent passwd ${USER_UID} | cut -d: -f1)"; \
    fi; \
    # sudoers e diretórios
    echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > "/etc/sudoers.d/${USERNAME}"; \
    chmod 0440 "/etc/sudoers.d/${USERNAME}"

# Troca de usuário OBRIGATÓRIA (antes de instalar o Oh My Zsh)
USER ${USERNAME}
ENV HOME=/home/${USERNAME}

# Instala o Oh My Zsh e os Plugins
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended
RUN git clone https://github.com/zsh-users/zsh-autosuggestions ${HOME}/.oh-my-zsh/custom/plugins/zsh-autosuggestions && \
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${HOME}/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting

# Baixa o tema Powerlevel10k
RUN git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ${HOME}/.oh-my-zsh/custom/themes/powerlevel10k

# Copia os arquivos de configuração customizados (ZSH e P10k)
COPY --chown=${USERNAME}:${USERNAME} docker/config/zshrc /home/${USERNAME}/.zshrc
COPY --chown=${USERNAME}:${USERNAME} docker/config/p10k.zsh /home/${USERNAME}/.p10k.zsh

# Define qual é o diretório padrão de trabalho
WORKDIR /home/${USERNAME}/ros_ws

# Etapa 5: Comando padrão para abrir o terminal (ZSH)
CMD ["/bin/zsh"]
