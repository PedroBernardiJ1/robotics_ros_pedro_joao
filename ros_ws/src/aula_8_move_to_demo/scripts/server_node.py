#!/usr/bin/env python3

# Bibliotecas do Sistema
import math
import time

# Bibliotecas do ROS
import rclpy
from rclpy.action import ActionServer, CancelResponse, GoalResponse
from rclpy.action.server import ServerGoalHandle
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node

# Bibliotecas Próprias/Customizadas
from geometry_msgs.msg import Pose
from aula_8_robot_interfaces.action import MoveTo

class MoveToServer(Node):

    # Atributos de classe ('self.' implícito) --> definidos fora de qualquer função para poderem ser acessados sempre
    MAX_DISTANCE = 10.0                                                                     # Alvos mais distantes que isso (em metros) são recusados
    TIMEOUT = 30.0                                                                          # Tempo maximo de execucao (em segundos) antes de abortar
    STEP = 0.5                                                                              # Quanto o robo "anda" por ciclo, em metros
    PERIOD = 0.5                                                                            # Duracao de cada ciclo, em segundos

    def __init__(self):

        super().__init__("move_to_server")                                                  # Nome não precisa ser o mesmo da classe, mas facilita visualização

        # Posicao atual do robo simulado
        self.x = 0.0
        self.y = 0.0

        # Aqui seria criada a Booleana comentada mais adiante (no 'goal_callback')

        self._action_server = ActionServer(
                            self,                                                           # Nó que hospeda o action server, no caso, é o próprio que estamos criando
                            MoveTo,                                                         # Tipo da action
                            "move_to",                                                      # Nome da action
                            execute_callback=self.execute_callback,
                            goal_callback=self.goal_callback,
                            cancel_callback=self.cancel_callback,
                            callback_group=ReentrantCallbackGroup(),
                            )

        self.get_logger().info("MoveTo action server started!")                             # Print de inicialização

    # ---------- helpers ----------                                                         # Funções Auxiliares para automatizar

    # Define a Pose atual do Robô
    def current_pose(self):

        pose = Pose()
        pose.position.x = self.x
        pose.position.y = self.y
        pose.position.z = 0.0
        pose.orientation.w = 1.0                                                            # Quaternion identidade

        return pose

    # Calcula a distância entre o GOAL e a Posição atual do robô
    def distance_to(self, target):

        return math.hypot(target.x - self.x, target.y - self.y)

    # Cria o 'result' automaticamente, pois sempre precisaremos dele na execução, mesmo que o GOAL seja cancelado/abortado
    def make_result(self, success, distance, start):

        result = MoveTo.Result()
        result.success = success
        result.final_distance = distance
        result.elapsed_time = time.time() - start

        return result

    # ---------- callbacks ----------

    # Callback de avalização do GOAL (não obrigatório)--> Será chamada assim que receber um GOAL --> "eu aceito essa tarefa?"
    def goal_callback(self, goal_request: MoveTo.Goal):                                     # Sempre recebe o 'goal_request'
        """Decide se aceitamos a goal, ANTES de comecar a executar."""

        distance = self.distance_to(goal_request.goal_pose.position)                      # Faz a requisição de um GOAL

        if distance > self.MAX_DISTANCE:

            self.get_logger().warn(f"Goal rejeitada: {distance:.2f} m e longe demais")      # Print de erro já no recebimento do GOAL

            return GoalResponse.REJECT                                                      # Nem inicializa a tarefa, já rejeita o GOAL passado

        # Poderia ter sido criado também um atributo booleano que verifica se
        # o servidor já recebeu um GOAL e aqui seria implementada a lógica para
        # verificar isso também. Dentre outras travas

        self.get_logger().info(f"Goal aceita: {distance:.2f} m ate o alvo")                 # Print de aceitação do GOAL

        return GoalResponse.ACCEPT

    # Callback de cancelamento (não obrigatório) --> Será chamada caso seja recebido um CANCEL (do Cliente) --> "eu aceito parar?"
    def cancel_callback(self, goal_handle: ServerGoalHandle):                               # Sempre recebe atributo do tipo 'goal_handle'
        """Decide se aceitamos o pedido de cancelamento."""

        self.get_logger().info("Cancelamento requisitado")                                  # Print de recebimento do cancelamento

        return CancelResponse.ACCEPT                                                        # Cancela

    # Callback de execução do GOAL (obrigatório) --> a lógica toda é implementada aqui
    def execute_callback(self, goal_handle: ServerGoalHandle):                              # Sempre recebe o atributo do tipo 'goal_handle' também
        """Executa a tarefa. Roda ate a goal terminar de um dos três jeitos."""

        goal: MoveTo.Goal = goal_handle.request
        target = goal.goal_pose.position
        tolerance = goal.tolerance

        feedback = MoveTo.Feedback()
        start = time.time()                                                                 # Inicializa o timer

        while True:                                                                         # Vai entrar no loop e as checagens serão feitas internamente

            distance = self.distance_to(target)

            # --- Checagens de Execução ---

            # 1. Chegamos no GOAL?
            if distance <= tolerance:

                goal_handle.succeed()                                                       # Finaliza a Action, com sucesso
                self.get_logger().info("Alvo alcancado")                                    # Mensagem no terminal caso a Action seja concluída com sucesso

                return self.make_result(True, distance, start)                              # 'make_result' para quando dá certo, recebendo distância e start (timer)

            # 2. O cliente pediu cancelamento?
            if goal_handle.is_cancel_requested:

                goal_handle.canceled()                                                      # Cancela a Action (por pedido do Cliente)
                self.get_logger().warn("Goal cancelada")                                    # Mensagem no terminal caso a Action seja cancelada

                return self.make_result(False, distance, start)                             # 'make_result' para quando é cancelada, recebendo distância e start (timer)

            # 3. Estamos demorando demais?
            if time.time() - start > self.TIMEOUT:

                goal_handle.abort()                                                         # Aborta a Action
                self.get_logger().error("Goal abortada: tempo esgotado")                    # Mensagem no terminal, caso a Action seja abortada pelo limite de tempo

                return self.make_result(False, distance, start)                             # 'make_result' para quando é abortada, recebendo distância e start (timer)

            # --- Lógica de movimentação do robô ---

            # Anda um passo na direcao do alvo
            dx = (target.x - self.x) / distance
            dy = (target.y - self.y) / distance
            step = min(self.STEP, distance)         # Define o Step como o mínimo entre STEP e distância do alvo

            self.x += step * dx                     # Atualiza o self.x
            self.y += step * dy                     # Atualiza o self.y

            # Publica o progresso depois do passo
            feedback.distance_remaining = self.distance_to(target)
            feedback.current_pose = self.current_pose()
            goal_handle.publish_feedback(feedback)

            time.sleep(self.PERIOD)                                                         # Deixa sempre o mesmo período entre execuções do While (não flooda o terminal)

def main():

    rclpy.init()

    node = MoveToServer()

    # Um executor com varias threads e obrigatorio aqui
    executor = MultiThreadedExecutor()
    rclpy.spin(node, executor=executor)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()

# "O código é uma arte!" - Zé, Zé - 2026
