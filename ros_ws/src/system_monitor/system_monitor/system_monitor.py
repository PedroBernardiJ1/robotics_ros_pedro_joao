import rclpy
import psutil
from rclpy.node import Node
from system_monitor_interfaces.msg import Cpu
from system_monitor_interfaces.msg import Ram
from system_monitor_interfaces.msg import Disco
from system_monitor_interfaces.msg import Monitoramento

class system_monitor (Node):

    def __init__(self):
        super().__init__("system_monitor")
        
        # Publisher
        self.system_monitor_publisher_ = self.create_publisher(Monitoramento, "hardware_status", 10)

        # Timers
        self.timer_publisher_ = self.create_timer(1.0, self.publisher_monitoramento)    

    def publisher_monitoramento(self):

        # Instanciando objeto do tipo Cpu
        cpu_computador = Cpu()
        cpu_computador.numero_cores = psutil.cpu_count(False)
        cpu_computador.uso_medio_total_cpu = psutil.cpu_percent(interval = None)
        cpu_computador.uso_individual_core = psutil.cpu_percent(interval = None, percpu=True)

        # Lógica de seleção da temperatura da CPU
        temps = psutil.sensors_temperatures() # Recebendo a tupla com os dados para temperatura da CPU

        sensores_coretemp = temps.get("coretemp") # Busca os sensores 'coretemp'

        # Define como temperatura da CPU o valor máximo encontrado dentre todos os "current" no "coretemp"
        cpu_computador.temperatura = float(max(sensor.current for sensor in sensores_coretemp))

        # # Lógica de seleção da maior temperatura
        # temperatura_current = 0.0
        # temperatura_high = 0.0
        # temperatura_critical = 0.0

        # array = temps.get("coretemp") # Busca os sensores 'coretemp'
        # for i in range (len(array)):
        #     for tupla in array:
        #         temperatura_current_i = tupla [1]
        #         temperatura_high_i = tupla [2]
        #         temperatura_critical_i = tupla [3]

        #         if (temperatura_current_i > temperatura_current):
        #             temperatura_current = temperatura_current_i

        #         if (temperatura_high_i > temperatura_high):
        #             temperatura_high = temperatura_high_i

        #         if (temperatura_critical_i > temperatura_critical):
        #             temperatura_critical = temperatura_critical_i

        # # Salvando o valor máximo de temperatura nos sensores
        # if (temperatura_critical >= temperatura_current) and (temperatura_critical >= temperatura_high):
        #     cpu_computador.temperatura = temperatura_critical

        # if (temperatura_current >= temperatura_critical) and (temperatura_current >= temperatura_high):
        #     cpu_computador.temperatura = temperatura_current

        # if (temperatura_high >= temperatura_critical) and (temperatura_high >= temperatura_current):
        #     cpu_computador.temperatura = temperatura_high

        # Instanciando objeto do tipo Ram
        ram_computador = Ram()
   
        tupla_ram = psutil.virtual_memory() # Recebendo a tupla auxiliar, com os dados para Ram

        # Instanciando atributos da custom_message a partir da tupla auxiliar
        ram_computador.uso = tupla_ram.percent
        ram_computador.memoria_total = tupla_ram.total
        ram_computador.memoria_utilizada = tupla_ram.used
        
        # Instanciando objeto do tipo Disco
        disco_computador = Disco()

        tupla_disco = psutil.disk_usage('/') # Recebendo a tupla auxiliar, com os dados para Disco

        # Instanciando atributos da custom_message a partir da tupla auxiliar
        disco_computador.porcentagem_uso = tupla_disco.percent
        disco_computador.espaco_disponivel = tupla_disco.total
        disco_computador.espaco_uso = tupla_disco.used

        # Instanciando objeto do tipo Monitoramento
        monitor = Monitoramento()

        # Header
        monitor.header.frame_id = "monitor_hardware"
        monitor.header.stamp = self.get_clock().now().to_msg() # Pegando o timestamp atual

        # Instanciando atributos da custom_message
        monitor.monitoramento_cpu = cpu_computador
        monitor.monitoramento_ram = ram_computador
        monitor.monitoramento_disco = disco_computador
        
        # Publicando
        self.system_monitor_publisher_.publish(monitor)

        self.get_logger().info(f"""
        --- Dados de CPU ---

        - Número de Cores: {cpu_computador.numero_cores}
        - Porcentagem média de uso da CPU: {cpu_computador.uso_medio_total_cpu}
        - Porcentagem de uso individual de cada core: {list(cpu_computador.uso_individual_core)}
        - Temperatura da CPU em graus Celsius: {cpu_computador.temperatura}

        --- Dados de RAM ---

        - Porcentagem de uso da RAM: {ram_computador.uso}
        - Quantidade total de memória (em bytes): {ram_computador.memoria_total}
        - Quantidade de memória utilizada (em bytes): {ram_computador.memoria_utilizada}

        --- Dados de Disco ---

        - Porcentagem de uso do disco: {disco_computador.porcentagem_uso}
        - Espaço total disponível (em bytes): {disco_computador.espaco_disponivel}
        - Espaço utilizado (em bytes): {disco_computador.espaco_uso}""")
        
def main():

    rclpy.init()

    node = system_monitor()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
