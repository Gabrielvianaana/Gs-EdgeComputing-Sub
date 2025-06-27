🌱 Projeto FloodSense - Medidor Inteligente de Umidade do Solo

📌 Descrição

Protótipo funcional baseado em Arduino Uno R3 para monitorar os níveis de umidade do solo, com indicações visuais e sonoras. Desenvolvido como parte da Global Solution (GS) da FIAP, o sistema visa alertar situações de seca ou encharcamento para aplicação em agricultura ou monitoramento ambiental.

🧰 Componentes Utilizados

Arduino Uno R3

Sensor de umidade do solo (simulado por slide potenciômetro)

Display LCD 16x2 com I2C

Módulo RTC (DS3231)

Buzzer (alerta sonoro)

LEDs (verde, amarelo, vermelho) com resistores 220Ω

EEPROM interna do Arduino

Protoboard e jumpers

⚙️ Funcionamento

Leitura da umidade do solo (simulada por potenciômetro)

Filtro exponencial para suavização de leitura

Três níveis de alerta visual:

Umidade alta (≥ 70%): LED verde + buzzer por 3 segundos

Umidade ideal (40–69%): LED amarelo

Umidade baixa (< 40%): LED vermelho + buzzer por 3 segundos

Display LCD exibe a umidade atual e o horário via módulo RTC

EEPROM armazena a menor umidade detectada

Histerese evita oscilações visuais e falsas leituras

🚨 Diferenciais

Alerta para excesso e escassez de umidade

Simulação de sensor com potenciômetro facilita testes

Registro histórico com EEPROM

Leitura suavizada e estável

Interface visual e sonora amigável


🎥 Demonstração: 

link wokwi: https://wokwi.com/projects/434931387195424769

link youtube: https://youtu.be/N5b3uO3jhJw


🔧 Como Utilizar

Conecte os componentes conforme o esquema

Carregue o código na IDE Arduino

Alimente o sistema e observe a leitura e alertas
