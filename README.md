# Autores:
Gabriel Lima Zambianco 
11201722750

Fabrício Expedito Albano Gaeta
21032616

# Descrição do projeto:
Ao longo das atividades apresentadas em computação gráfica, abordamos diversos conceitos, partindo de aplicação interativa com gráficos 2D, passando por aplicação gráfica 3D com animações, e por fim, chegando em aplicação gráfica 3D com iluminação e texturização. Ambas foram aplicadas utilizando shaders de iluminação e texturização e é interativa através do mouse e teclado. 

Para o desenvolvimento da aplicação, partimos da ideia de planetário e começamos a destrinchar novas possibilidades, permanecendo no âmbito do sistema solar. O Sistema Solar é um conjunto formado por oito planetas e outros corpos celestes, que orbitam o Sol, a sua principal estrela. Está localizado na Via Láctea, uma das galáxias que formam o Universo. São planetas do Sistema Solar: Mercúrio, Vênus, Terra, Marte, Júpiter, Saturno, Urano e Netuno. Além dos planetas, temos também satélites sendo a Lua o satélite natural mais conhecido, que órbita na Terra. No nosso projeto, aplicamos as técnicas aprendidas no curso em uma classificação específica dos planetas do sistema solar, os planetas rochosos, conhecidos também como planetas telúricos, planetas sólidos ou terrestres. São planetas rochosos assim como a Terra e possuem maior densidade se comparados com os planetas gasosos. Os planetas rochosos do Sistema Solar são:
•	Mercúrio;
•	Vênus;
•	Terra;
•	Marte (na ordem de afastamento do Sol). 

Além desses planetas, também inserimos o Sol e a Lua em órbita da Terra. 

Esse projeto tem o propósito de simular os planetas rochosos do sistema solar, a ideia de escala entre os tamanhos dos elementos, mostrar graficamente os elementos do Sistema Solar em uma perspectiva 3D, explorar os conceitos apresentados em aula.

# Aplicação:
O usuário consegue interagir com a aplicação através do scroll do mouse, onde é possível aproximar ou afastar da aplicação e ao segurar o botão direito rotacionar pelo espaço. Além disso, é possível interagir através das teclas do teclado, movendo a câmera, onde: 
•	W: Ir para cima
•	S: Ir para baixo
•	D: Ir para direita
•	E: Ir para esquerda 
Também é possível visualizar a aplicação em fullscreen.  

> Classe Main: Semelhante aos projetos anteriores, alterando basicamente o nome da aplicação.

> Classe Solar: Desenha o corpo celeste com sua posição e tamanho, considerando principalmente órbita e escala. Dentro desta classe, temos funções que criam as esferas e os buffers a serem passados ao OpenGL (create), destrói os buffers já utilizados (destroy), desenha o corpo na cena (render), vetor dos vértices das esferas, escala de tamanho do corpo, cor do corpo celeste (color) e raio da órbita do corpo ao redor do  corpo satélite (orbit_radius). 

> Classe Window: Semelhante aos demais projetos, definimos o funcionamento geral da cena da UI, passando por exemplo os objetos a serem criados. 

> Classe Camera: Permite que o usuário mova a câmera na cena, capturando o cursor do mouse e realizando a rotação, movimentação de aproximação ou afastamento, movendo a câmera verticalmente e horizontalmente. 

> Texturização: Para cada um dos planetas rochosos, Sol e Lua, utilizamos a textura difusa que representam as cores de sua superfície. Todas as texturas utilizadas foram obtidas na Internet. 
