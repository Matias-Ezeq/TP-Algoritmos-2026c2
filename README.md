# TP Algoritmos 2026c2

Ejecutar desde el directorio raiz con el siguiente comando:

./out/(nombreDelPrograma)

### Análisis de Carga de Datos Ordenados en el ABB

#### Pregunta: 
¿Qué altura tendría el árbol si las bicicletas se cargaran ordenadas por ID? ¿Cómo afecta eso al costo de la búsqueda? (Ejemplo con 5 bicicletas).

#### Análisis de Altura y Ejemplo
Si las bicicletas se insertan en el árbol en un orden estrictamente secuencial u ordenado por ID (por ejemplo: `10, 20, 30, 40, 50`), el Árbol Binario de Búsqueda pierde su balance y degenera en una estructura lineal (equivalente a una lista enlazada).

* **Ejemplo con 5 bicicletas (IDs: 10, 20, 30, 40, 50):**
  * La altura de un árbol perfectamente balanceado con 5 nodos sería de **3 niveles**.
  * Sin embargo, al cargarse ordenadas, cada nodo nuevo se cuelga exclusivamente de la rama derecha del anterior. Su altura resultante será de **5 niveles** (igual a la cantidad total de elementos $N$).

#### Impacto en el Costo de la Búsqueda
* **En un árbol balanceado:** La complejidad temporal de búsqueda es logarítmica, expresada como $O(\log n)$.
* **En un árbol degenerado (lineal):** La complejidad temporal empeora drásticamente y pasa a ser lineal, expresada como $O(n)$.

Para buscar el último elemento (ID 50), el sistema se verá obligado a recorrer uno por uno todos los nodos anteriores, perdiendo por completo la ventaja de eficiencia que caracteriza a los árboles de búsqueda.
