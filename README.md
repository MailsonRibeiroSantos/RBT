# RBT

Descrição projeto: <br/>
O projeto consiste na implementação de árvore binária de busca(BST) de forma genérica utilizando template. Temos como interface da nossa árvore os seguintes métodos: 
<ul>
<li>const DataType & find_min() const;</li>
<li>const DataType & find_max() const;</li>
<li>bool contains( const KeyType& key ) const;</li>
<li>size_type size() const;</li>
<li>bool empty() const;</li>
<li>bool retrieve( const KeyType& key, DataType& value ) const;</li>
<li>bool validate() const;</li>
<ul>
<br/>
Para compilar o código o usuário deve acessar a pasta via terminal e digitar o seguinte comando "make". Para executar o usuário deve entrar no seguinte caminho /BST/build/bin e executar o seguinte comando:  "./driver_rbt" 
