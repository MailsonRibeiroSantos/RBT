#include <cassert>
#include "../include/rbt.h"
#include <iostream>
#include <stack>      // std::stack

template < typename KeyType, typename ValueType>
RBT<KeyType, ValueType>::~RBT(){
    clear();
    delete m_root;
    //delete m_current;
    delete null_node;
}

template < typename KeyType, typename ValueType>
bool RBT<KeyType, ValueType>::validate( ) const {
  if(not_double_red(m_root) && root_is_black() && count_blacks(m_root)!=-1)
    return true;
  return false;
}

template < typename KeyType, typename ValueType>
RBT<KeyType, ValueType>:: RBT( const RBT & other ){
    m_n_nodes = other.m_n_nodes;
    m_key_less = other.m_key_less;
    //clear(); //deleta a árvore caso ele já possua uma.
    m_root = clone(other.m_root);
    null_node = m_root->parent;
    m_current=nullptr;
}


 template < typename KeyType, typename ValueType>
  RBT<KeyType, ValueType> &  RBT<KeyType, ValueType>:: operator =( const RBT & rhs ){
      m_key_less = rhs.m_key_less;
      clear(); //deleta a árvore caso ele já possua uma.
      m_n_nodes = rhs.m_n_nodes; //atualiza o m_n_nodes depois do clear() pq o remove() decrementa o m_n_nodes
      m_root = clone(rhs.m_root);
      null_node = m_root->parent;
      m_current=nullptr;

      return *this; //retorna o próprio BST
  }


 template < typename KeyType, typename ValueType>
 typename RBT<KeyType, ValueType>::RBNode *  RBT<KeyType, ValueType>::clone(  RBNode * root ){
      RBNode *newRoot;
      RBNode *newNullNode;
      newRoot = new RBNode (root->key,root->data,root->color);
      newNullNode = new RBNode (0,0,RBT::RBNode::color_e::BLACK);
      newNullNode->left=newNullNode; newNullNode->right=newNullNode; newNullNode->parent=newNullNode;
      newRoot->parent=newNullNode;

      std::stack < RBNode *> s;
      std::stack <RBNode *> sClone;


      s.push(root);
      sClone.push(newRoot);

      while(!s.empty()){


          auto visited = s.top(); s.pop();
          auto visitedClone = sClone.top(); sClone.pop();

          if(visited->right!=root->parent){ //o root->parent é o nullNode do argumento

               visitedClone->right = new RBNode (visited->right->key,visited->right->data,visited->right->color,newNullNode,newNullNode,visitedClone);
               s.push(visited->right);
               sClone.push(visitedClone->right);
          }
          if(visited->left!=root->parent){
              visitedClone->left = new RBNode (visited->left->key,visited->left->data,visited->left->color,newNullNode,newNullNode,visitedClone);
              s.push(visited->left);
              sClone.push(visitedClone->left);
          }
      }

      return newRoot;
  }



template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::rotate( RBNode * root ){
    if(root->parent->parent->left==root->parent){
        rotate_right_child(root);
        rotate_left_child(root);

    }else if(root->parent->parent->right==root->parent){

        rotate_left_child(root);
        rotate_right_child(root);
    }
}

template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::rotate_left_child( RBNode * root ){
    RBNode *aux;
    aux=root->parent->parent;//cria um auxiliar pra apontar pro avo

    if(m_root==root->parent){ //se o pai de root for a raiz principal, o root passará a ser a raiz principal depois da rotaćao
        m_root=root;
    }
   // std::cout<<"raiz principal: "<<m_root->key<<std::endl;
    root->parent->left=root->right; //o pai aponta pra o filho direito do nó em análise
   // std::cout<<"novo filho do pai: "<<root->parent->left->key<<std::endl;
     if(root->right!=null_node){
     root->right->parent=root->parent; //o pai do filho direito agora vai ser o nó que era pai do nó em análise
   //  std::cout<<"novo pai do filho direito: "<<root->right->parent->key<<std::endl;
     }
    root->right=root->parent;
  //  std::cout<<"novo filho direito: "<<root->right->key<<std::endl;
    if(root->parent->parent!=null_node){
        if(root->parent->parent->right==root->parent)
            root->parent->parent->right=root;
        else{
            root->parent->parent->left=root;
        }
    }

    root->right->parent=root;
    root->parent=aux;

}


template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::rotate_right_child( RBNode * root ){
    RBNode *aux;
    aux=root->parent->parent;//cria um auxiliar pra apontar pro avo

    if(m_root==root->parent){
        m_root=root;
    }
    root->parent->right=root->left; //o pai aponta pra o filho esquerdo do nó em análise

    if(root->left->parent!=null_node){
    root->left->parent=root->parent; //o pai do filho esquerdo(caso este nao seja o no nulo) agora vai ser o nó que era pai do nó em análise
    }
    root->left=root->parent; //o filho esquerdo agora será o nó que era seu pai
    if(root->parent->parent!=null_node){
        if(root->parent->parent->right==root->parent)
            root->parent->parent->right=root;
        else{
            root->parent->parent->left=root;
        }

    }

    root->left->parent=root; //o nó em analise será o pai do seu novo filho esquerdo q antes era seu pai
    root->parent=aux;
}
template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::insert( RBNode * & root, const KeyType & key, const ValueType & value ){
    if(root==nullptr){
        null_node = new RBNode (0,0,RBT::RBNode::color_e::BLACK,null_node,null_node,null_node);
        root = new RBNode (key,value,RBT::RBNode::color_e::BLACK,null_node,null_node,null_node);
       // m_root=root;
        m_n_nodes++;
        m_current=root;

    }else{
        if(!contains( key )){
            if(key>root->key){                if(root->right==null_node){
                    root->right = new RBNode (key,value,RBT::RBNode::color_e::RED,null_node,null_node,root);
                    m_n_nodes++;
                    m_current=root->right;
                    //std::cout<<m_current->data<<std::endl;
                    if(m_current->parent->parent!=null_node){
                      rearrange();
                    }
                }
                else{
                    insert(root->right,key,value);
                }
            }
            else{
                if(root->left==null_node){
                    root->left = new RBNode (key,value,RBT::RBNode::color_e::RED,null_node,null_node,root);
                    m_n_nodes++;
                    m_current=root->left;
                    if(m_current->parent->parent!=null_node){
                       rearrange();
                    }
                }
                else{
                    insert(root->left,key,value);
                }
            }
        }

    }
}

template < typename KeyType, typename ValueType >
void RBT<KeyType, ValueType>::insert( const KeyType & key , const ValueType & value ){

    insert(m_root,key,value);

}


template < typename KeyType, typename ValueType >
bool RBT<KeyType, ValueType>::contains( const RBNode * root , const KeyType & key ) const{
    if(root!=null_node){ //se a raiz for difetente de nulo, ele busca

                    if(root->key== key){ //caso ache a chave, retorna verdadeiro
                            return true;
                    }
                    else if(key>root->key){ //caso a chave seja maior, ele busca recursivamente no filho a direita

                            return contains( root->right, key);
                    }
                    else{ //caso a chave seja menor, ele busca recursivamente no filho a esquerda
                            return contains( root->left, key);
                    }
    }
    else{
        return false;
     }
 }

template < typename KeyType, typename ValueType>
bool RBT<KeyType, ValueType>::contains( const KeyType & key ) const{
   return contains(m_root,key);
}

template < typename KeyType, typename ValueType>
typename RBT<KeyType, ValueType>::RBNode * RBT<KeyType, ValueType>::successor( RBNode * root ) const{
    RBNode * sucessor;
    sucessor=root;
    if (root->right!=null_node){
        sucessor=root->right;
        while(sucessor->left!=null_node){
            sucessor=sucessor->left;
        }
    }else{
        //std::cout<<"Não tem sucessor\n ";
    }
    return sucessor;
}

template < typename KeyType, typename ValueType>
typename  RBT<KeyType, ValueType>::RBNode * RBT<KeyType, ValueType>::predecessor( RBNode * root ) const {
    RBNode * predecessor;
    predecessor=root;
    if (root->left!=null_node){
        predecessor=root->left;
        while(predecessor->right!=null_node){
            predecessor=predecessor->right;
        }
    }else{
        //std::cout<<"Não tem predecessor\n ";
    }
    return predecessor;
}

template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::remove(  RBNode * & root, const KeyType & key){

   if(root!=null_node){

        if(root->key==key){

            RBNode * aux;
            aux=successor(root);

            root->key=aux->key;
            root->data=aux->data;
            if(aux->right!=null_node){ //se o filho direito do sucessor for diferente do nó nulo, ele só pode ser um nó vermelho
                aux->key=aux->right->key; //o nó vai recever a chave do filho
                aux->data=aux->right->data;
                delete aux->right; //o filho é deletado
                aux->right=null_node;
                aux->color=RBT::RBNode::color_e::BLACK;
                m_n_nodes--;

            }else if(aux->left!=null_node){//caso nao tenha filho direito, mas tenha filho esquerdo
                aux->key=aux->left->key; //o nó vai recever a chave do filho
                aux->data=aux->left->data;
                delete aux->left; //o filho é deletado
                aux->left=null_node;
                aux->color=RBT::RBNode::color_e::BLACK;
                 m_n_nodes--;

            }else{ //caso nao tenha nenhum dos dois filhos
                if(aux==m_root){// caso o unico elemento seja a raiz

                    m_root=nullptr;

                    delete aux;
                    delete null_node;
                    null_node=nullptr;

                    m_n_nodes--;
                }else{
                    if(aux->color==RBT::RBNode::color_e::RED){//Se for vermelho basta deletar
                        if(aux->parent->left==aux){//se for filho esquerdo, deleta o esquerdo
                            aux->parent->left=null_node;
                            delete aux;
                             m_n_nodes--;
                        }else{
                            aux->parent->right=null_node;
                            delete aux;
                             m_n_nodes--;
                        }


                    }else{
                        aux->color=RBT::RBNode::color_e::DOUBLEBLACK; //marca o nó a ser removido como doubleblack
                        m_current=aux;
                        rearrangeAfterRemotion();
                    }
                }
            }
        }
        else if(key>root->key){
            remove(root->right,key);
        }
        else{
            remove(root->left,key);
        }
   }
}
template < typename KeyType, typename ValueType>
void  RBT<KeyType, ValueType>::remove( const KeyType & key ){
    remove(m_root,key);
}

template < typename KeyType, typename ValueType>
void  RBT<KeyType, ValueType>::rearrangeAfterRemotion( void){
    if(m_current!=m_root){
        if(m_current==m_current->parent->left){ //quando o irmão for o filho direito de seu pai
            if(m_current->parent->right->color==RBT::RBNode::color_e::RED){ //caso 1
                rotate_right_child(m_current->parent->right);
                m_current->parent->parent->color=RBT::RBNode::color_e::BLACK;
                m_current->parent->color=RBT::RBNode::color_e::RED;
                rearrangeAfterRemotion();
            }
            else if(m_current->parent->right->color==RBT::RBNode::color_e::BLACK){//caso 2
                if(m_current->parent->right->right->color==RBT::RBNode::color_e::BLACK && m_current->parent->right->left->color==RBT::RBNode::color_e::BLACK){
                     m_current=m_current->parent;
                     m_current->right->color=RBT::RBNode::color_e::RED;
                    if(m_current->left->color==RBT::RBNode::color_e::DOUBLEBLACK){
                        delete m_current->left;
                        m_n_nodes--;
                        m_current->left=null_node;

                    }
                    //std::cout<<"caso 2 "<<std::endl;
                    if(m_current->color==RBT::RBNode::color_e::RED){ //caso o novo nó seja vermelho, o caso é terminal e ele fica preto
                        m_current->color=RBT::RBNode::color_e::BLACK;
                    }else{//caso não terminal, chama a funćao novamente
                         rearrangeAfterRemotion();
                    }

                }
                else if(m_current->parent->right->right->color==RBT::RBNode::color_e::BLACK && m_current->parent->right->left->color==RBT::RBNode::color_e::RED){
                    rotate_left_child(m_current->parent->right->left);
                    m_current->parent->right->right->color=RBT::RBNode::color_e::RED;
                    m_current->parent->right->color=RBT::RBNode::color_e::BLACK;
                    rearrangeAfterRemotion();
                }
                else if(m_current->parent->right->right->color==RBT::RBNode::color_e::RED){//caso 4
                    m_current->parent->right->color=m_current->parent->color;
                    m_current->parent->color=RBT::RBNode::color_e::BLACK;
                    m_current->parent->right->right->color=RBT::RBNode::color_e::BLACK;
                    rotate_right_child(m_current->parent->right);

                    if(m_current->color==RBT::RBNode::color_e::DOUBLEBLACK){
                        m_current->parent->left=null_node;
                        delete m_current;
                        m_n_nodes--;
                    }
                }
            }

        }
        else if(m_current==m_current->parent->right){ //caso simetrico, quando o irmao for filho esquerdo do pai
            if(m_current->parent->left->color==RBT::RBNode::color_e::RED){ //caso 1
                rotate_left_child(m_current->parent->left);
                m_current->parent->parent->color=RBT::RBNode::color_e::BLACK;
                m_current->parent->color=RBT::RBNode::color_e::RED;
                rearrangeAfterRemotion();
            }
            else if(m_current->parent->left->color==RBT::RBNode::color_e::BLACK){
                if(m_current->parent->left->right->color==RBT::RBNode::color_e::BLACK && m_current->parent->left->left->color==RBT::RBNode::color_e::BLACK){//caso2
                    m_current=m_current->parent; //o nó em análise passa a ser o pai
                    m_current->left->color=RBT::RBNode::color_e::RED; //o filho esquerdo fica vermelho
                   if(m_current->right->color==RBT::RBNode::color_e::DOUBLEBLACK){//se for caso inicial, o nó é removido
                       delete m_current->right;
                       m_current->right=null_node;
                       m_n_nodes--;
                   }
                   if(m_current->color==RBT::RBNode::color_e::RED){ //caso o novo nó seja vermelho, o caso é terminal e ele fica preto
                       m_current->color=RBT::RBNode::color_e::BLACK;
                   }else{//caso não terminal, chama a funćao novamente
                        rearrangeAfterRemotion();
                   }

                }
                else if(m_current->parent->left->right->color==RBT::RBNode::color_e::RED && m_current->parent->left->left->color==RBT::RBNode::color_e::BLACK){//caso3
                    rotate_right_child(m_current->parent->left->right);
                    m_current->parent->left->left->color=RBT::RBNode::color_e::RED;
                    m_current->parent->left->color=RBT::RBNode::color_e::BLACK;
                    rearrangeAfterRemotion();
                }
                else if(m_current->parent->left->left->color==RBT::RBNode::color_e::RED){//caso 4
                    m_current->parent->left->color=m_current->parent->color;
                    m_current->parent->color=RBT::RBNode::color_e::BLACK;
                    m_current->parent->left->left->color=RBT::RBNode::color_e::BLACK;
                    rotate_left_child(m_current->parent->left);
                     if(m_current->color==RBT::RBNode::color_e::DOUBLEBLACK){
                        m_current->parent->right=null_node;
                        delete m_current;
                        m_n_nodes--;
                     }
                }
            }

        }
    }
}

template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::rearrange( void){
    if(m_current->parent->parent->left==m_current->parent){//quando o pai é filho esquerdo do avo
        if(m_current->parent->color==RBT::RBNode::color_e::RED){//se o pai for vermelho tem que reorganizar a árvore

            if(m_current->parent->parent->right->color==RBT::RBNode::color_e::RED){//quando o tio for vermelho
                m_current->parent->parent->right->color=RBT::RBNode::color_e::BLACK;
                m_current->parent->parent->color=RBT::RBNode::color_e::RED;
                m_current->parent->color=RBT::RBNode::color_e::BLACK;
                if(m_root== m_current->parent->parent)//caso seja raiz, ela tem q voltar a ser preto
                    m_current->parent->parent->color=RBT::RBNode::color_e::BLACK;
                else{
                    m_current=m_current->parent->parent; //o nó atual passa a ser o novo nó vermelho
                    rearrange(); //e chama novamente a funcao para chegar se o pai do novo nó corrente eh vermelho
                }

            }
            else if(m_current->parent->parent->right->color==RBT::RBNode::color_e::BLACK){//se o tio for preto, temos dois casos
                if(m_current==m_current->parent->right){
                    rotate(m_current);
                    m_current->color=RBT::RBNode::color_e::BLACK;
                    m_current->right->color=RBT::RBNode::color_e::RED;
                }else{
                    m_current->parent->parent->color=RBT::RBNode::color_e::RED;
                    m_current->parent->color=RBT::RBNode::color_e::BLACK;
                    rotate_left_child(m_current->parent);
                }
            }

        }
    }
    else if(m_current->parent->parent->right==m_current->parent){ //quando o pai é filho direito do avo

        if(m_current->parent->color==RBT::RBNode::color_e::RED){//se o pai for vermelho tem que reorganizar a árvore

            if(m_current->parent->parent->left->color==RBT::RBNode::color_e::RED){ //quando o tio for vermelho

                m_current->parent->parent->left->color=RBT::RBNode::color_e::BLACK;
                m_current->parent->parent->color=RBT::RBNode::color_e::RED;
                m_current->parent->color=RBT::RBNode::color_e::BLACK;
                if(m_root== m_current->parent->parent)//caso seja raiz, ela tem q voltar a ser preto
                    m_current->parent->parent->color=RBT::RBNode::color_e::BLACK;
                else{
                    m_current=m_current->parent->parent; //o nó atual passa a ser o novo nó vermelho
                    rearrange(); //e chama novamente a funcao para chegar se o pai do novo nó corrente eh vermelho
                }
            }
            else if(m_current->parent->parent->left->color==RBT::RBNode::color_e::BLACK){

                if(m_current==m_current->parent->left){ //forma joelho

                    rotate(m_current);
                    m_current->color=RBT::RBNode::color_e::BLACK;
                    m_current->left->color=RBT::RBNode::color_e::RED;
                }else{

                    m_current->parent->parent->color=RBT::RBNode::color_e::RED;
                    m_current->parent->color=RBT::RBNode::color_e::BLACK;
                    rotate_right_child(m_current->parent);
                }
            }

        }
    }
}

template < typename KeyType, typename ValueType>
const ValueType & RBT<KeyType, ValueType>::find_min( void) const{
    RBNode *aux;
    aux=m_root;
    while(aux->left!=null_node){
        aux=aux->left;
    }
    return aux->data;
}

template < typename KeyType, typename ValueType>
const ValueType & RBT<KeyType, ValueType>::find_max(void ) const{
    RBNode *aux;
    aux=m_root;
    while(aux->right!=null_node){
        aux=aux->right;
    }
    return aux->data;
}

template < typename KeyType, typename ValueType>
bool RBT<KeyType, ValueType>::retrieve(const RBNode * root, const KeyType & key, ValueType & value ) const {
    if(root!=null_node){
                    if(root->key== key){
                            value = root->data;
                            return true;
                    }
                    else if(key>root->key){
                            return retrieve(root->right, key, value);
                    }
                    else{
                            return retrieve(root->left, key, value);
                    }
    }
    else{
    return false;
     }
}

template < typename KeyType, typename ValueType>
bool RBT<KeyType, ValueType>::retrieve( const KeyType & key, ValueType & value ) const{
   return retrieve(m_root,key,value);
}



template<typename KeyType , typename ValueType >
template<typename UnaryFunction >
void RBT< KeyType, ValueType >::inorder    (   const UnaryFunction &   visit   )   const
{
    inorder(m_root,visit);
}
template<typename KeyType , typename ValueType>
template<typename UnaryFunction >
void RBT< KeyType, ValueType>::inorder( const RBNode*  root,const UnaryFunction &      visit ) const
{
     if ( root != null_node )
    {
        inorder( root->left, visit);
        visit( root->data );
        inorder( root->right, visit);
    }
}
//preorder
template<typename KeyType , typename ValueType >
template<typename UnaryFunction >
void RBT< KeyType, ValueType>::preorder ( const UnaryFunction &  visit)   const
{
    preorder(m_root,visit);
}
template<typename KeyType , typename ValueType  >
template<typename UnaryFunction >
void RBT< KeyType, ValueType >::preorder (const RBNode*  root,const UnaryFunction &  visit) const
{
    if (root!= null_node  )
    {
        visit( root->data );
        preorder( root->left, visit);
        preorder( root->right, visit);
    }
}
//posorder
template<typename KeyType , typename ValueType >
template<typename UnaryFunction >
void RBT< KeyType, ValueType>::postorder (const UnaryFunction &  visit)const
{
    postorder(m_root,visit);
}
template<typename KeyType , typename ValueType >
template<typename UnaryFunction >
void RBT< KeyType, ValueType>::postorder  (const RBNode*  root, const UnaryFunction &  visit ) const
{
     if ( root != null_node  )
    {
        postorder( root->left, visit);
        postorder( root->right, visit);
        visit( root->data );
    }

}


template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::clear( RBNode * & root ){
    while(root!=nullptr){ //enquanto a raiz nao for nula

        remove(root->key); //continua removendo a chava que ta na raiz

    }
}

template < typename KeyType, typename ValueType>
void RBT<KeyType, ValueType>::clear( void ){
    clear(m_root);
}

template<typename KeyType , typename ValueType >
bool RBT< KeyType, ValueType>::operator==( const RBT& other ) const
{

    if((*this).m_n_nodes != other.m_n_nodes)
            return false;
    else if((*this).m_n_nodes ==0 && other.m_n_nodes==0)
                        return true;
    std::vector<ValueType> v_save_1;
    auto save_1 = [&]( const ValueType & dado)-> void
    {
        v_save_1.push_back(dado);
    };
    std::vector<ValueType> v_save_2;
    auto save_2 = [&]( const ValueType & dado)-> void
    {
       v_save_2.push_back(dado);
    };

    (*this).inorder(save_1);
    other.inorder(save_2);

    for (size_t i = 0; i < v_save_1.size(); ++i)
    {
        if(v_save_1[i]!=v_save_2[i]){
            return false;
        }
    }
    v_save_1.clear();
    v_save_2.clear();
    return true;
}
template<typename KeyType , typename ValueType >
bool RBT< KeyType, ValueType>::not_double_red(const RBNode * root) const
{
  if(root != null_node)
  {
    if(root->color==RBT::RBNode::color_e::RED &&( root->right->color==RBT::RBNode::color_e::RED || root->left->color==RBT::RBNode::color_e::RED))
    {
      return false;
    }else
    {
      not_double_red(root->left);
      not_double_red(root->right);
    }

  }
  return true;
}
template<typename KeyType , typename ValueType >
bool RBT< KeyType, ValueType>::root_is_black( ) const 
{
  if(m_root!=null_node){
    if(m_root->color==RBT::RBNode::color_e::BLACK)
      return true;
    return false;
  }
  return true;
}

template<typename KeyType , typename ValueType >
int  RBT< KeyType, ValueType>::count_blacks( const RBNode * root ) const {
    if(root != null_node)
	{
    int left =count_blacks(root->left); 
    int right = count_blacks(root->right);
    if(left!=right || right==-1 || left== -1)
    {
      return -1;
    }
		else if(root->color==RBT::RBNode::color_e::BLACK)
			return 1 + left;
		else if(root->color==RBT::RBNode::color_e::RED)
			return  left;

	}else
  {
	 return 1;
  }
}





