#include <iostream>   // cout, cin, endl
#include <utility>    // std::pair<>
#include <random>     // radom_device, mt19937, std::shuffle.
#include <iomanip>    // std::setw()
#include <string>     // std::string
#include <limits>     // numeric_limits::lowest
#include <cassert>
#include <vector>
#include <algorithm>

#include "../include/rbt.h"

int main()
{
    auto n_unit{0}; // unit test count.

    // Our RBT will store integer with integer keys.
    typedef std::pair< size_t,size_t > test_type;

    // Input data
    test_type data[] = {
        { 10, 10 },
        { 85, 85 },
        { 15, 15 },
        { 70, 70 },
        { 20, 20 },
        { 60, 60 },
        { 30, 30 },
        { 50, 50 },
        { 65, 65 },
        { 80, 80 },
        { 90, 90 },
        { 40, 40 },
        {  5,  5 },
        { 55, 55 }
    };

    // Print the input
    {
        // The tree declaration.
#define USE_FUNCTOR
#ifdef USE_FUNCTOR
        struct CompareKey {
            bool operator()( const size_t & a, const size_t & b )
            {
                return a < b ;
            }
        } cmp;

        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest(), cmp );
#else // Use lambda
        auto compare_keys = []( const size_t &a, const size_t &b )->bool
        {
            return a < b ;
        };
        RBT< size_t, size_t > tree( compare_keys );
#endif

#ifdef PRINT
        std::cout << ">>> Inserting data:\n";
        for( const auto & e : data )
        {
            std::cout << "< " << std::setw(3) << e.first << " , \""
                << e.second << "\" >\n";
            tree.insert( e.first, e.second );
            assert( tree.validate() );
            std::cout << "\n>>> The tree:\n" << tree << std::endl;
        }
        //std::cout << "\n>>> The tree:\n" << tree << std::endl;
#endif
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": insertion.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );

        for( const auto & e : data )
        {
            tree.insert( e.first, e.second );
            assert( tree.validate() );
        }
        for( const auto & e : data )
            assert( tree.contains( e.first ) );

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": deletion.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );

        // Fill in the tree.
        for( const auto & e : data )
            tree.insert( e.first, e.second );
        assert( tree.validate() );

        // Remove each individual node from the complete original tree.
        // However, at each new loop iteration the original tree is restored.
        auto backup_tree = tree; // save original tree.
        for ( const auto & e : data )
        {
            tree = backup_tree; // restore back the original tree.
            tree.remove( e.first ); // Remove a single node.
            assert( tree.validate() ); // Check whether everything is fine with the tree.
        }

        // Progressively removes all the tree nodes until it becomes empty.
        for( const auto & e : data )
        {
            //std::cout << "Removing: < " << std::setw(3) << e.first << " , \"" << e.second << "\" >\n";
            tree.remove( e.first );
            //std::cout << "\n>>> [main()] The tree:\n" << tree << std::endl;
            assert( tree.validate() );
        }

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": find_min.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );

        auto min{ data[0].first };
        for( const auto & e : data )
        {
            // Get the smallest so far.
            min = std::min( min, e.first );
            tree.insert( e.first, e.second );
            assert( tree.find_min() == min );
        }

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": find_max.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );

        auto max{ data[0].first };
        for( const auto & e : data )
        {
            // Get the smalles so far.
            max = std::max( max, e.first );
            tree.insert( e.first, e.second );
            assert( tree.find_max() == max );
        }

        std::cout << ">>> Passed!\n\n";
    }

    // Testing tree traversal
    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": tree traversal.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );

        // Insert elements.
        for( const auto & e : data )
            tree.insert( e.first, e.second );
        assert( tree.validate() );

        // Expected answers
        std::vector< decltype(data[0].second) > pre_vec =
        { 30, 15, 10, 5, 20, 70, 60, 50, 40, 55, 65, 85, 80, 90 };
        std::vector< decltype(data[0].second) > in_vec =
        { 5, 10, 15, 20, 30, 40, 50, 55, 60, 65, 70, 80, 85, 90 };
        std::vector< decltype(data[0].second) > pos_vec =
        { 5, 10, 20, 15, 40, 55, 50, 65, 60, 80, 90, 85, 70, 30 };

        // function object.
        std::vector< decltype(data[0].second) > vec;
        auto print = [&]( const decltype(data[0].second) & data_ )-> void
        {
            vec.push_back ( data_ ); // Capture data.
            //std::cout << data_ << ", ";
        };

        vec.clear();
        //std::cout << ">>> preorder tree traversal:\n";
        tree.preorder( print );
        assert( vec == pre_vec );

        vec.clear();
        //std::cout << "\n>>> inorder tree traversal:\n";
        tree.inorder( print );
        assert( vec == in_vec );

        vec.clear();
        //std::cout << "\n>>> postorder tree traversal:\n";
        tree.postorder( print );
        assert( vec == pos_vec );

        std::cout << ">>> Passed!\n\n";
    }
    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": size.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );

        size_t expected_size{1};
        for( const auto & e : data )
        {
            tree.insert( e.first, e.second );
            assert( expected_size == tree.size() );
            ++expected_size;
        }
        assert( tree.validate() );

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": empty.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );
        assert( true == tree.empty() );

        for( const auto & e : data )
        {
            tree.insert( e.first, e.second );
            assert( false == tree.empty() );
        }
        assert( tree.validate() );

        for( const auto & e : data )
        {
            //std::cout << "Removing: < " << std::setw(3) << e.first << " , \"" << e.second << "\" >\n";
            tree.remove( e.first );
            //std::cout << "\n>>> [main()] The tree:\n" << tree << std::endl;
            assert( tree.validate() );
        }
        assert( true == tree.empty() );

        std::cout << ">>> Passed!\n\n";
    }
    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": clear.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );
        assert( true == tree.empty() );

        for( const auto & e : data )
            tree.insert( e.first, e.second );
        assert( tree.validate() );

        tree.clear();
        assert( true == tree.empty() );

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": retrieve.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );
        assert( true == tree.empty() );

        for( const auto & e : data )
            tree.insert( e.first, e.second );
        assert( tree.validate() );

        for( const auto & e : data )
        {
            size_t value;
            assert( tree.retrieve( e.first, value ) );
            assert( value == e.second );
        }

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": contains.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );
        assert( true == tree.empty() );

        for( const auto & e : data )
            tree.insert( e.first, e.second );
        assert( tree.validate() );

        for( const auto & e : data )
        {
            assert( tree.contains( e.first ) );
            assert( tree.validate() );
        }

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": operator==().\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );
        assert( true == tree.empty() );
        RBT< size_t, size_t > tree2( std::numeric_limits<size_t>::lowest() );
        assert( true == tree2.empty() );

        // Empty tree must be equal.
        assert( tree == tree2 );

        // Insert data in both trees, so they are equal.
        for( const auto & e : data )
        {
            tree.insert( e.first, e.second );
            tree2.insert( e.first, e.second );
        }
        assert( tree.validate() );
        assert( tree2.validate() );

        // equal operator.
        assert( tree2 == tree );
        assert( tree2.size() == tree.size() );

        // Let us remove just one element from one of the trees.
        tree.remove( data[0].first );
        assert( not( tree2 == tree ) );
        assert( tree != tree2 );

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": assignment operator.\n";
        // The tree
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::lowest() );
        assert( true == tree.empty() );

        for( const auto & e : data )
            tree.insert( e.first, e.second );
        assert( tree.validate() );

        // Assignment operator.
        auto tree2 = tree;

        // Let us test if everything is ok with the clone tree.
        assert( tree2.validate() == true );
        for( const auto & e : data )
            assert( tree2.contains( e.first ) );

        std::cout << ">>> Passed!\n\n";
    }

    {
        std::cout << ">>> Unit teste #" << ++n_unit << ": copy constructor.\n";
        // Creating a tree with > instead of <.
        RBT< size_t, size_t > tree( std::numeric_limits<size_t>::max(),
                []( const size_t& a, const size_t& b )->bool{ return a>b;});
        assert( true == tree.empty() );

        for( const auto & e : data )
            tree.insert( e.first, e.second );
        assert( tree.validate() );

        // Assignment operator.
        auto tree_copy( tree );

        tree.clear();

        // Let us test if everything is ok with the clone tree.
        assert( tree_copy.validate() == true );
        for( const auto & e : data )
            assert( tree_copy.contains( e.first ) );

        std::cout << ">>> Passed!\n\n";
    }

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}
