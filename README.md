# Gs-EdgeComputing-Sub

🌱 Projeto FloodSense - Medidor Inteligente de Umidade do Solo

📌 Descrição

Protótipo funcional baseado em Arduino Uno R3 para monitorar os níveis de umidade do solo, com indicações visuais e sonoras. Desenvolvido como parte da Global Solution (GS) da FIAP, o sistema visa alertar situações de seca ou encharcamento para aplicação em agricultura ou monitoramento ambiental.

🧰 Componentes Utilizados

Arduino Uno R3

Sensor de umidade do solo (capacitivo ou analógico)

Display LCD 16x2 com I2C

Módulo RTC (DS3231)

Buzzer

LEDs (verde, amarelo, vermelho)

Resistores 220Ω

EEPROM interna

Protoboard e jumpers

⚙️ Funcionamento

Leitura da umidade do solo com filtro exponencial

Três níveis de alerta:

Alta umidade (≥ 70%): LED verde + buzzer

Moderada (40% a 69%): LED amarelo

Baixa (< 40%): LED vermelho + buzzer

Display LCD exibe umidade atual e horário (RTC)

EEPROM registra a menor umidade detectada

Histerese evita oscilações abruptas

🚨 Diferenciais

Alerta para excesso e escassez de umidade

Registro histórico com EEPROM

Leitura suavizada e estável

Interface de fácil compreensão e operação

Código limpo, comentado e modularizado


🔧 Como Utilizar

Conecte os componentes conforme o esquema

Carregue o código na IDE Arduino

Alimente o sistema e observe a leitura e alertas
