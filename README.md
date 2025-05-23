
# Projeto: Monitoramento de Condições Ideais para Armazenamento de Vinhos

## 🎯 Descrição

Este projeto consiste em um sistema automatizado que monitora as condições ideais de temperatura, umidade e luminosidade para o armazenamento de vinhos tintos e brancos. **O grande diferencial deste projeto é a possibilidade de selecionar entre dois tipos de vinho: tinto ou branco**, ajustando automaticamente os parâmetros ideais de conservação conforme a escolha.

A seleção do tipo de vinho é feita por botões físicos, e o sistema se adapta com base nas faixas ideais para cada um, proporcionando um cuidado especializado e mais preciso na armazenagem.

Além disso, o sistema utiliza um display LCD para informar o status em tempo real, LEDs para indicar se as condições estão boas ou não, um buzzer para alertas críticos e grava médias de temperatura, umidade e luminosidade na memória EEPROM do Arduino.

## ⚙️ Componentes Utilizados

- **Arduino Uno**: Microcontrolador principal.
- **Sensor DHT22**: Mede temperatura e umidade.
- **Sensor LDR**: Mede luminosidade ambiente.
- **Display LCD 16x2 com I2C**: Exibe informações de status.
- **RTC DS3231**: Mantém a hora e data atualizadas.
- **EEPROM**: Memória interna do Arduino utilizada para salvar médias.
- **Buzzer**: Emite sons de alerta quando as condições são críticas.
- **LEDs (Verde, Amarelo, Vermelho)**: Indicadores visuais de status.
- **2 Botões**: Seleção entre vinho tinto e branco — **o grande diferencial do sistema**.
- **Protoboard e fios**: Para realizar as conexões.

## 🖼️ Diagrama de Montagem

> O diagrama apresentado acima mostra todas as ligações entre os componentes.  
Destaques importantes:
- O **DHT22** está conectado ao pino digital 2.
- O **LDR** está ligado à entrada analógica A0.
- O **LCD** utiliza comunicação I2C (pinos A4 - SDA e A5 - SCL).
- Os **LEDs** são conectados nos pinos 4, 5 e 6.
- O **buzzer** está no pino 7.
- Os **botões** estão nos pinos 11 (Branco) e 12 (Tinto).

## 💡 Funcionalidades

### ✅ Seleção de Tipo de Vinho (**Diferencial do Projeto**)
- **Botão Tinto**: Define faixas ideais para vinho tinto (12-18 °C, 60-70% UR).
- **Botão Branco**: Define faixas ideais para vinho branco (8-12 °C, 70-80% UR).

Esta seleção torna o sistema **altamente personalizado e eficiente** no cuidado com diferentes tipos de vinho.

### ✅ Monitoramento de Ambiente
- **Temperatura** e **Umidade**: Lidas com o sensor DHT22.
- **Luminosidade**: Lida com LDR.
- Valores exibidos no monitor serial e LCD.

### ✅ Indicadores
- **LED Verde**: Condições ideais.
- **LED Amarelo**: Condições fora da faixa, mas não críticas.
- **LED Vermelho e Buzzer**: Alerta crítico.

### ✅ Memória EEPROM
- Armazena as médias de temperatura, umidade e luminosidade a cada 10 segundos.

### ✅ Exibição no LCD
- Mostra o tipo de vinho selecionado.
- Mostra mensagens como "Tudo OK", "Aumente Temp", "Reduza Lum.".
- Apresenta uma animação com ícones personalizados.

### ✅ Data e Hora
- Utiliza o módulo RTC DS3231 para manter registros temporais precisos.

## 🚨 Regras de Atuação

- Se **todos os parâmetros** estiverem ideais → **LED Verde** ligado.
- Se algum parâmetro estiver fora da faixa → **LED Amarelo**.
- Se temperatura estiver muito fora da faixa → **LED Vermelho** + **Buzzer**.

## 📊 Faixas Ideais

| Tipo de Vinho | Temperatura (°C) | Umidade Relativa (%) | Máx. Luminosidade (%) |
|---------------|------------------|----------------------|-----------------------|
| Tinto         | 12 a 18          | 60 a 70              | 30                    |
| Branco        | 8 a 12           | 70 a 80              | 30                    |

## 📝 Pontos Interessantes

- **Diferencial**: seleção entre tipos de vinho, adaptando parâmetros automaticamente.
- Utilização da EEPROM para persistência de dados.
- Controle de estado com enumeração (`TipoVinho`).
- Mapeamento da luminosidade com `map()`.
- Ícones personalizados no LCD.
- Prevenção de múltiplos acionamentos do buzzer.

## ✅ Possíveis Melhorias Futuras

- Inclusão de um módulo WiFi (ESP8266/ESP32) para monitoramento remoto.
- Ajuste das faixas ideais via interface física.
- Gravação de logs históricos em cartão SD.
- Tela LCD maior para mais informações.

## 💻 Bibliotecas Utilizadas

- `Wire.h` → Comunicação I2C.
- `RTClib.h` → Comunicação com módulo RTC.
- `LiquidCrystal_I2C.h` → Controle do display LCD.
- `DHT.h` → Controle do sensor DHT22.
- `EEPROM.h` → Manipulação da memória EEPROM.

## 🚧 Como Montar

1. Monte o circuito conforme o diagrama.
2. Carregue o código no Arduino.
3. Abra o monitor serial em 9600 baud.
4. Observe o comportamento:
   - Pressione os botões para definir o tipo de vinho.
   - Acompanhe as leituras no serial e no display.
   - Verifique a atuação dos LEDs e buzzer conforme variação ambiental.

## 🎯 Objetivo Educacional

Esse projeto é excelente para aprender sobre:
- Monitoramento ambiental.
- Utilização de sensores.
- Persistência de dados com EEPROM.
- Controle de hardware através de software.
- Exibição de informações em LCD.


## 🖥️ Como Rodar este Projeto

### ✅ Simulação Online com Wokwi

Este projeto pode ser simulado de forma totalmente online através da plataforma **Wokwi**, que permite testar circuitos de Arduino sem a necessidade de componentes físicos.

Acesse o projeto diretamente neste link:  
➡️ **[Simular no Wokwi](https://wokwi.com/projects/431491020123528193)**

### 🚀 Passos para rodar:

1. Clique no link acima.
2. Aguarde o carregamento do ambiente de simulação.
3. Pressione o botão **"Start Simulation"**.
4. Utilize os botões virtuais no ambiente para selecionar entre **Vinho Tinto** e **Vinho Branco**.
5. Observe:
   - O comportamento dos LEDs.
   - A exibição no display LCD.
   - O acionamento do buzzer conforme variações nas leituras.
6. Modifique os valores de temperatura, umidade e luminosidade ajustando os controles do DHT e do LDR no painel lateral.

### ✅ Vantagens do Wokwi

- Não precisa de Arduino físico.
- Testa rapidamente mudanças no código.
- Visualiza o funcionamento completo do sistema.
- Compartilha facilmente com outras pessoas.

Créditos
Desenvolvido por: 
Kauê de Almeida Pena - 564211, 
Gabriel Ferreira Machado - 562330, 
João Paulo Santana Basta - 565383, 
Alexandre Wesley - 561622, 
João Stellare - 565813 
Equipe: Data-Vine 
Instituição: Vinheria Agnello 
Simulado com: Wokwi Arduino Simulator Ano: 2025
