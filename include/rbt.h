#ifndef RBT_H
#define RBT_H

   #include <iostream>   // cout, cin, endl
 #include <functional> // std::less<>()
   #include <utility>    // std::pair<>
   #include <random>     // radom_device, mt19937, std::shuffle.
    #include <algorithm>  // std::copy
    #include <iterator>   // std::begin, std::end
    #include <exception>  // std::out_of_range
    #include <stack>      // std::stack
  #include <iomanip>    // std::setw()
    #include <initializer_list> // std::initializer_list


template < typename KeyType, typename ValueType>
class RBT
{
    public:
         typedef std::function<bool(const KeyType& a, const KeyType& b) > KeyTypeLess;
    private :

        //! Represents a single binary search tree node.
        struct RBNode {
            enum class color_e{ RED, BLACK, DOUBLEBLACK }; //!< The colors.
            KeyType key;
            //!< The unique key.
            ValueType data; //!< The data stored in a node.
            color_e color; //!< The color of the node.
            RBNode *left;
            //!< Pointer to the left child (subtree).
            RBNode *right; //!< Pointer to the right child (subtree).
            RBNode *parent; //!pointer to the parent
            //! Default constructor for a tree node.
            RBNode( const KeyType & k_=KeyType(), const ValueType & d_=ValueType(),
                    color_e c_=color_e::RED, RBNode * lt_=nullptr, RBNode * rt_=nullptr, RBNode * parent_=nullptr )
                : key( k_ )
                , data( d_ )
                , color (c_)
                , left( lt_ )
                , right( rt_ )
                , parent (parent_)

                { /* empty */ }

            friend std::ostream& operator<< ( std::ostream& os_, const RBNode & n )
                            {
                                  os_ << "< " << n.key << " >"
                                   << " , color = " << ( (n.color==RBNode::color_e::RED)?"R":"B")
                                   << ", @" << &n ;
                                 return os_;
                               }
        };



        RBNode *m_root; //!< Dummy node that represents the root of the entire tree.
        size_t m_n_nodes; //!< The count of nodes stored in the tree.
        KeyTypeLess m_key_less; //!< The key comparator function object.

        //=== Aux pointers to help the operations.
        RBNode *m_current;//!< Current node being processed.
       // RBNode *m_parent;//!< The parent node of ‘m_current‘.
        //=== Extra black node to make the code simpler.
        RBNode *null_node; //!< Every null pointer of this tree points to this node.

public:
           //=== alias
         typedef std::pair<KeyType, ValueType> node_content_type;
         typedef std::pair<typename RBNode::color_e, typename RBNode::color_e > pair_colors_type;
    private :
        //=== INTERNAL MEMBERS
        void init( const KeyType & smallest_key, const KeyTypeLess & comp ) ;
       /* RBNode * rotate_left_child( RBNode * root ) ;
        RBNode * rotate_right_child( RBNode * root ) ;
        RBNode * rotate( RBNode * root ) ;*/

       void rotate_left_child( RBNode * root ) ;
        void rotate_right_child( RBNode * root ) ;
        void rotate( RBNode * root ) ;
        void insert( RBNode * & root, const KeyType & key, const ValueType & value );
        void rearrange( void) ;
        void rearrangeAfterRemotion( void) ;
        void clear( RBNode * & root );
        RBNode * clone(RBNode * root );
        void remove(RBNode * & root, const KeyType & key  );
      //  const RBNode * successor( const RBNode * root ) const ;
      //  const RBNode * predecessor( const RBNode * root ) const ;

        bool retrieve(const RBNode * root, const KeyType & key, ValueType & value ) const;

        RBNode * successor(  RBNode * root ) const ;
        RBNode * predecessor(  RBNode * root ) const ;
        bool validate_tree( const RBNode * root ) const ;
        int count_blacks( const RBNode * root ) const ;
        bool key_equal( const KeyType & k1, const KeyType & k2 ) const ;
        bool contains( const RBNode * root , const KeyType & key ) const;
        //=== Tree traversal members

        template < typename UnaryFunction >
        void preorder( const RBNode * root, const UnaryFunction & visit ) const;
        template < typename UnaryFunction >
        void inorder( const RBNode * root, const UnaryFunction & visit ) const;
        template < typename UnaryFunction >
        void postorder( const RBNode * root, const UnaryFunction & visit ) const;


    public :
        //=== special member
      /*  explicit RBT( const KeyType & smallest_key, const KeyTypeLess & comp = KeyTypeLess())
            :m_root(nullptr)
            ,null_node(new RBNode (0,0,RBT::RBNode::color_e::BLACK,null_node,null_node,null_node))
            ,m_n_nodes(0)
            ,m_key_less(comp)
            ,m_current(nullptr)
        {  }*/
        explicit RBT( const KeyType & smallest_key, const KeyTypeLess & comp = KeyTypeLess())
                    :m_root(nullptr)
                    ,m_n_nodes(0)
                    ,m_key_less(comp)
                    ,m_current(nullptr)
                    ,null_node(nullptr)                  
                {  }

        RBT( const RBT & other ) ;
        ~RBT();
        RBT & operator =( const RBT & rhs ) ;
        bool operator==( const RBT& other ) const ;
        bool operator!=( const RBT& other ) const { return not (*this == other); }
        //=== access members
        const ValueType & find_min( void) const;
        const ValueType & find_max(void ) const;

        bool contains( const KeyType & key ) const;


        template < typename UnaryFunction >
        void preorder( const UnaryFunction & visit ) const;
        template < typename UnaryFunction >
        void postorder( const UnaryFunction & visit ) const;
        template < typename UnaryFunction >
        void inorder( const UnaryFunction & visit ) const;

        inline bool empty(void ) const { return m_n_nodes == 0; }
        inline size_t size( void) const { return m_n_nodes; }

        bool retrieve( const KeyType & key, ValueType & value ) const ;
        bool validate( ) const ;
        bool root_is_black( ) const ;
        bool not_double_red(const RBNode * root) const;
       

        //=== MODIFIER MEMBERS

        void clear( void );
        void insert( const KeyType & key , const ValueType & value );
        void remove( const KeyType & key );

        //=== FRIEND FUNCTION

                  friend std::ostream& operator<< ( std::ostream& os_, const RBT& root_ )
                   {
                      if ( root_.empty() )
                      {
                          os_ << "<empty tree>";
                          return os_;
                      }

                       typedef std::pair< size_t, RBNode* > stack_type;
                       std::stack< stack_type > s;
                       // We have at least one element in the tree.
                       // Remember: root points to a dummy node that has the entire tree on its right branch.
                      // s.push( std::make_pair( 0, root_.m_root->right ) ); // push root at level 0.
                        s.push( std::make_pair( 0, root_.m_root) ); // push root at level 0.
                     // We follow pre-order traversal strategy.
                    while( not s.empty() )
                       {
                           // (1) Visite the root first.
                         auto visited = s.top(); s.pop();
                           if ( visited.second == root_.null_node )
                           {
                             // os_ << "X:"<< visited.second->data  << " ("
                              //     << ( (visited.second->color==RBNode::color_e::RED)?"R":"B") << ")\n";
                              continue;
                           }
                           else
                           {
                          for( auto i(0u) ; i < visited.first ; ++i )
                              os_ << "--";
                               os_ << visited.second->data  << " ("
                                   << ( (visited.second->color==RBNode::color_e::RED)?"R":"B") << ")\n";
                           }

                          s.push( std::make_pair( visited.first+1, visited.second->right ) );
                          s.push( std::make_pair( visited.first+1, visited.second->left ) );
                      }

                       os_ << "root: " << root_.m_root->data << " ("
                          << ( (root_.m_root->color==RBNode::color_e::RED)?"R":"B") << ")\n";
                      os_ << "null_node: " << root_.null_node->data << " ("
                          << ( (root_.null_node->color==RBNode::color_e::RED)?"R":"B") << ")\n";
                    return os_;
                   }
};

#include "rbt.inl"
#endif

































