import matplotlib.pyplot as plt
import numpy as np

# Dados dos testes
testes = np.arange(1, 11)
udp_loss = [
    47.93, 38.55, 44.96, 40.59, 53.28, 42.77, 45.39, 39.38, 44.57, 52.81
]

# Gráfico de perda de pacotes no UDP
plt.figure(figsize=(10, 6))
plt.bar(testes, udp_loss, color='orange', label='Perda de Pacotes (%)')
plt.title('Perda de Pacotes no Protocolo UDP', fontsize=14)
plt.xlabel('Testes', fontsize=12)
plt.ylabel('Perda de Pacotes (%)', fontsize=12)
plt.xticks(testes)
plt.legend()
plt.grid(axis='y')

# Salvar e mostrar o gráfico
plt.savefig('perda_pacotes_udp.png')
plt.show()
