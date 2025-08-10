# Batalha Naval - Jogo em C com WinAPI

## Descrição

Este é um jogo simples de **Batalha Naval** desenvolvido em linguagem C utilizando a biblioteca **WinAPI** para interface gráfica no Windows. O jogador enfrenta a máquina em um tabuleiro personalizável, podendo escolher a dificuldade e o tamanho do campo.

---

## Funcionalidades

- Tabuleiro ajustável: 5x5, 10x10 ou 15x15 células  
- Três níveis de dificuldade da IA (Fácil, Médio, Difícil)  
- Jogo baseado em turnos: jogador e máquina alternam jogadas  
- Interface gráfica com botões para disparar nos campos da IA  
- Indicação visual clara:  
  - Água: azul  
  - Acerto: vermelho  
  - Erro: cinza  
- Mensagens simples para vitória ou derrota  
- Menu para iniciar nova partida, ajustar dificuldade, tamanho e sair do jogo  

---

## Como jogar

- Compile o código com o compilador GCC (MinGW) ou outro compatível.  
- Execute o programa gerado.  
- Use o menu para configurar dificuldade e tamanho do tabuleiro.  
- Clique nas células para tentar acertar os navios da IA.  
- Aguarde a jogada da IA após a sua vez.  
- O jogo termina quando todos os navios de um lado forem atingidos.  

---

## Como compilar

Use o seguinte comando no terminal (Windows com MinGW):

```bash
gcc batalha_naval_winapi.c -o batalha_naval.exe -mwindows
```

## Requisitos

- Sistema operacional Windows  
- Compilador C com suporte a WinAPI (ex: MinGW)  

---

## Estrutura do Código

- `WinMain` — inicialização da janela principal  
- `WindowProc` — tratamento dos eventos da janela (cliques, desenho, menu)  
- Funções para criar/remover botões conforme tamanho do tabuleiro  
- Função para posicionar navios aleatoriamente  
- Lógica de turnos e verificação de vitória  
- Interface gráfica simples, porém funcional  

---

## Possíveis melhorias

- Melhorar IA para buscar navios depois do primeiro acerto  
- Implementar animações e sons  
- Mostrar tabuleiro do jogador na interface  
- Adicionar opção de jogar contra outro jogador  
- Interface gráfica mais moderna com bibliotecas externas  
