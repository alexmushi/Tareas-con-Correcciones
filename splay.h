/*
 * splay.h
 *
 *  Created on: 6/11/2023
 *      Author: Jesús Alejandro Cedillo Zertuche
 */

#ifndef SPLAY_H_
#define SPLAY_H_

#include <string>
#include <sstream>

using namespace std;

template <class T> class SplayTree;

template <class T>
class Node {
private:
	T value;
	Node *left, *right, *parent;

	Node<T>* succesor();
    Node<T>* rot_right(Node<T>* x);
    Node<T>* rot_left(Node<T>* x);
public:
    // Constructor
	Node(T val) : value(val), left(0), right(0), parent(0) {}

    // Funciones a implementar
    Node<T>* add(T);
    Node<T>* find(T);
    Node<T>* remove(T);

	// Funciones auxiliares
    Node<T>* splay(Node<T>*, Node<T>*);
	void removeChilds();

    // Funciones auxiliares de impresion
    void print_tree(stringstream &aux) const;
    void preorder(stringstream &aux) const;
    void inorder(stringstream &aux) const;

	// Para que acceda a funciones
	friend class SplayTree<T>;
};

// Función de agregar
template <class T>
Node<T>* Node<T>::add(T val){
    // Comparas el valor a insertar contra el valor del nodo
	if (val < value){
		// Si no es nulo, se vuelve a llamar la función con recursión. 
		if (left != 0){
			// Aquí el left es el hijo, y se llama la función para que haga la comparación
			return left -> add(val);
		}
		// Si es Nulo, ahí se agrega y crea el objeto
		else {
			left = new Node<T>(val);
            // Inicializas quien es el padre del nodo (haces que el parent de left, ya que ahorita estas en nodo de arriba)
            left -> parent = this;
            return left;
		}
	}
	// Si no, se inserta a la derecha y se hace lo mismo, donde se inserta hasta que llege a nulo
	else {
		if (right != 0){
			return right -> add(val);
		}
		else {
			right = new Node<T>(val);
            // Inicializas quien es el padre del nodo (haces que el parent de left, ya que ahorita estas en nodo de arriba)
            right -> parent = this;
            return right;
		}
	}
}

template <class T>
Node<T>* Node<T>::find(T val){
    // El caso base donde si lo encontró
	if (val == value){
        // Regresar el nodo donde esta el valor
		return this;
	}
	// Si es menor recorre hacia la izquierda
	else if (val < value){
		// Si es nulo, llegó al final y regresas ese nodo
		if (left == 0){
			return this;
		}
		// Si no es nulo, vas a left y llamas a la recursión con ese nodo
		else {
			return left -> find(val);
		}
	}
	// Si es mayor reocorre hacia la derecha
	else {
		// Si es nulo, llegó al final y regresas ese nodo
		if (right == 0){
			return this;
		}
		// Si no es nulo, vas al hijo de right y llamas a la recursión con ese nodo
		else {
			return right -> find(val);
		}
	}
}

template <class T>
Node<T>* Node<T>::succesor(){
	// En este caso, son casos diferentes al normal
	Node<T> *le, *ri;
	le = left;
	ri = right;

	// Si es una hoja del árbol, regresar null
	if (right == 0 && left == 0){
		return 0;
	}

	// Si el árbol no tiene elemento al right
	if (right == 0) {
		if (left != 0) {
			// Para el caso extremo, si left tiene un hijo en right, lo pones para que no se pierda
			if (left -> right){
				// Como sabes que right no existe, lo pones para que se conecte en el remove
				right = left -> right;
			}
			// Otro caso en donde se escribe para no perder elementos
			left = left -> left;
		}
		// Regresar el primero que está a la izquierda
		return le;
	}

	// Si en este caso, el right no tiene left (ya que es el numero que se busca)
	if (right -> left == 0) {
		// Cambias la flecha del de antes al elemento que sigue 
		right = right -> right;
		// Le quitas la flecha al sucesor
		ri -> right = 0;
		// Regresas el elemento
		return ri;
	}

	// Creas los nodos para guardar el que es el sucesor
	Node<T> *par, *child;
	// Como estas buscando el número más chico de la derecha haces que el parent sea left
	par = right;
	// Haces que el child sea el objeto a la izquierda del padre
	child = right->left;
	// Este while recorre hasta llegar a la rama del árbol (el elemento más chico de right)
	while (child->left != 0) {
		par = child;
		child = child->left;
	}
	/* Si en el último elemento, tiene un hijo en right, haces que la flecha left 
	    del parent (elemento arriba al borrar) apunte al right del que ya es sucesor
		Esto se hace para no perder elementos y que todo este ligado en el árbol
	*/ 
	par -> left = child -> right;
	// Ya que ligaste el elemento, sobreescribes el right de child para que el elemento no tenga flechas
	child -> right = 0;
	// Regresas el sucesor
	return child;
}

template <class T>
Node<T>* Node<T>::remove(T val) {
	// Creas nodos para guardar datos
	Node<T> *succ, *old;

	// Como el valor que estás buscando es más chico, te mueves hacia la izquierda
	if (val < value) {
		// Checas que haya elemento
		if (left != 0) {
			// Si el left (elemento de abajo) ya tiene el valor que buscas
			if (left -> value == val) {
				// Hacer old (elemento que se va a borrar) el left, ya que tiene el valor
				old = left;
				// Buscarle un succesor a left con la función
				succ = left -> succesor();
				// Si si existe un succesor, atar las flechas antes de borrar elemento
				if (succ != 0) {
					// Hacer que el left del sucesor apunte hacia donde apuntaba el left del root
					succ -> left = old -> left;
					// Hacer lo mismo con el right
					succ -> right = old -> right;
					// Como el succesor cambia de lugar, su papá es el papá de old
					succ -> parent = old -> parent;

					// Si succesor tiene un hijo en left
					if (succ -> left)
						// Haces que el padre de ese hijo sea el succesor
						succ -> left -> parent = succ;
					// Si succesor tiene un hijo en right
					if (succ -> right)
						// Haces que el padre de ese hijo sea el succesor
						succ -> right -> parent = succ;
				}
				// Cambiar la flecha del left al sucesor antes de borrar elemento
				left = succ;
				// Borrar el elemento
				delete old;
				// Regresar el padre (que como se trabaja es el nodo actual)
				return this;
			} 
			// Si no es igual al valor, llamas a la recursión con el elemento de abajo (left)
			else {
				return left -> remove(val);
			}
		}
		// Si se pide borrar un elemento que no existe, regresar el último nodo para hacer el splay
		else {
			return this;
		}
	} 
	// Como el valor que estas buscando es más grande, te mueves hacia la derecha
	else {
		// Checas que haya elemento
		if (right != 0) {
			if (right -> value == val) {
				// Hacer old (elemento que se va a borrar) el right, ya que tiene el valor
				old = right;
				// Buscarle un succesor a right con la función
				succ = right -> succesor();
				// Si si existe un succesor, atar las flechas antes de borrar elemento
				if (succ != 0) {
					// Hacer que el left del sucesor apunte hacia donde apuntaba el left del root
					succ -> left = old -> left;
					// Hacer lo mismo con el right
					succ -> right = old -> right;
					// Como el succesor cambia de lugar, su papá es el papá de old
					succ -> parent = old -> parent;

					// Si succesor tiene un hijo en left
					if (succ -> left)
						// Haces que el padre de ese hijo sea el succesor
						succ -> left -> parent = succ;
					// Si succesor tiene un hijo en right
					if (succ -> right)
						// Haces que el padre de ese hijo sea el succesor
						succ -> right -> parent = succ;
				}
				// Cambiar la flecha del left al sucesor antes de borrar elemento
				right = succ;

				// Borrar el elemento
				delete old;
				// Regresar el padre
				return this;
			} 
			// Si no es igual al valor, llamas a la recursión con el elemento de abajo (right)
			else {
				return right -> remove(val);
			}
		}
		// Si se pide borrar un elemento que no existe, regresar el último nodo para hacer el splay
		else {
			return this;
		}
	}
}

template <class T>
Node<T>* Node<T>::rot_right(Node<T>* x){
	// Como es rotación a la derecha el nodo a rotar es y
    Node<T> *y = x -> left;
	// Cambias la flecha de x a y 
	x -> left = y -> right;
	// Si "y" si tiene un hijo en right
	if(y -> right)
		// Haces que el parent de ese hijo ahora sea igual a x
		y -> right -> parent = x;

	// Como sube el nodo y rota a la derecha, el right de y ahora es el nodo de x
	y -> right = x;

	// Como subes a y, ahora su padre es el padre de x
	y -> parent = x -> parent;
	// Como bajas a x, ahora su papá es y
	x -> parent = y;

	// Unes la flecha del padre con y
	if(y -> parent){
		// Si el padre estaba unido desde la izquierda
		if(y -> parent -> left && y -> parent -> left -> value == x -> value)
			y -> parent -> left = y;
		// Si no estaba unido desde la derecha y lo unes desde ahí
		else
			y -> parent -> right = y;
	}
	return y;
}

// Aquí aplica lo mismo pero en reversa porque es rotación a la izquierda
template <class T>
Node<T>* Node<T>::rot_left(Node<T>* x){
    Node<T> *y = x -> right;
	x -> right = y -> left;
	if(y -> left)
		y -> left -> parent = x;

	y -> left = x;

	// enlace de y con el padre de x
	y -> parent = x -> parent;
	x -> parent = y;

	// hace enlace del padre de x a y
	if(y -> parent){
		if(y -> parent -> left && y -> parent -> left -> value == x -> value)
			y -> parent -> left = y;
		else
			y -> parent -> right = y;
	}
	return y;
}

template <class T>
Node<T>* Node<T>::splay(Node<T>* root, Node<T>* x){
    // Mientras el nodo insertado/buscado siga teniendo padre o no sea la raíz
    while (x -> parent != 0){
        // En estos casos solo es cuando el padre ya es la raíz
        if (x -> parent -> value == root -> value){
            // Checas que exista, y que lo que se vaya a rotar sea lo de la izquierda
            if (x -> parent -> left && x -> parent -> left -> value == x -> value){
                // Como esta en left, lo rotas a la derecha
                rot_right(x -> parent);
            }
            // Si no es, entonces es lo de la derecha
            else {
                // Como esta en right, lo rotas a la izquierda
                rot_left(x -> parent);
            }
        }
        else {
            // El padre del nodo que se hace raíz
            Node<T>*p = x -> parent;
            // El abuelo del nodo que se hace raíx
            Node<T>*g = p -> parent;

            // Aseguras que todo lo que se va a rotar tiene solo hijos a la izquierda, para rotarlos a la derecha
            if (p -> left && g -> left && 
                x -> value == p -> left -> value && p -> value == g -> left -> value){
                // Rotas al abuelo, y luego al padre para que el nuevo nodo este arriba
                rot_right(g);
                rot_right(p);
            }
            // Aseguras que todo lo que se va a rotar tiene solo hijos a la derecha, para rotarlos a la izquierda
            else if (p -> right && g -> right && 
                x -> value == p -> right -> value && p -> value == g -> right -> value){
                // Rotas al abuelo, y luego al padre para que el nuevo nodo este arriba
                rot_left(g);
                rot_left(p);
            }
            // Cuando el abuelo tiene hijo a la derecha, y el padre a la izquierda
            else if (p -> left && g -> right && 
                x -> value == p -> left -> value && p -> value == g -> right -> value) {
                /* Aquí es parecido a la doble rotación del AVL
                 * Primero rotas el padre a la derecha para que el nodo x quede en medio
                 * Ya que esta en el medio, rotas el abuelo a la izquierda para que x quede arriba
                 */
                rot_right(p);
                rot_left(g);
            }
            // Cuando el abuelo tiene hijo a la izquierda, y el padre a la derecha
            else {
                /* Aquí es parecido a la doble rotación del AVL
                 * Primero rotas el padre a la izquierda para que el nodo x quede en medio
                 * Ya que esta en el medio, rotas el abuelo a la derecha para que x quede arriba
                 */
                rot_left(p);
                rot_right(g);
            }
        }
    }
    // Regresas el nodo
    return x;
}

// Vas borrando todo hasta llegar al final
template <class T>
void Node<T>::removeChilds(){
	if (left != 0) {
		left -> removeChilds();
		delete left;
		left = 0;
	}
	if (right != 0) {
		right-> removeChilds();
		delete right;
		right = 0;
	}
}

template <class T>
void Node<T>::print_tree(stringstream &aux) const {
	if (parent != 0){
		aux << "\n node " << value;
		aux << " parent " << parent -> value;
		}
    else
		aux << "\n root " << value;
	if (left != 0)
		aux << " left " << left->value;
	if (right != 0)
		aux << " right " << right->value;
	aux << "\n";
	if (left != 0) {
		left->print_tree(aux);
	}
	if (right != 0) {
		right->print_tree(aux);
	}
}
	
template <class T>
void Node<T>::preorder(stringstream &aux) const {
	aux << value;
	if (left != 0) {
		aux << " ";
		left->preorder(aux);
	}
	if (right != 0) {
		aux << " ";
		right->preorder(aux);
	}
}
	
template <class T>
void Node<T>::inorder(stringstream &aux) const {
	if (left != 0) {
		left->inorder(aux);
	}
	if (aux.tellp() != 1) {
		aux << " ";
	}
	aux << value;
	if (right != 0) {
		right->inorder(aux);
	}
}

// Este es el árbol completo que tiene el apuntador a la raíz
template <class T>
class SplayTree {
private:
	Node<T> *root;

public:
    // Constructor
	SplayTree();
	~SplayTree();

    // Funciones simples
    bool empty() const;
	void removeAll();

    // Funciones a implementar
	void add(T);
	bool find(T);
	void remove(T);

    // Funciones auxiliares
    string print_tree() const;
	string inorder() const;
	string preorder() const;
};

// Creas el arbol donde inicias la raíz a 0
template <class T>
SplayTree<T>::SplayTree() : root(0) {}

template <class T> 
SplayTree<T>::~SplayTree(){
	removeAll();
}

template <class T>
bool SplayTree<T>::empty() const{
    return (root == 0);
}

template <class T>
void SplayTree<T>::add(T val){
    if (root != 0){
        Node <T>* added = root -> add(val);
        root = root -> splay(root, added);
    }
    else {
        root = new Node<T>(val);
    }
}

template <class T>
void SplayTree<T>::remove(T val){
	// Si el árbol existe
	if (root != 0) {
		// Si el valor que se busca borrar es el del root
		if (val == root -> value) {
			Node<T> *succ = root -> succesor();
			// Si encontró un succesor
			if (succ != 0) {
					// Hacer que el left del sucesor apunte hacia donde apuntaba el left del root
					succ -> left = root -> left;
					// Hacer lo mismo con el right
					succ -> right = root -> right;
					// Como el succesor es el nuevo root, su padre ahora es 0
					succ -> parent = 0;

					// Si succesor tiene un hijo en left
					if (succ -> left)
						// Haces que el padre de ese hijo sea el succesor
						succ -> left -> parent = succ;
					// Si succesor tiene un hijo en right
					if (succ -> right)
						// Haces que el padre de ese hijo sea el succesor
						succ -> right -> parent = succ;
			}
			// Borras la raíz y haces que la nueva raíz sea el succesor
			delete root;
			root = succ;
		} 
		// Si el valor que se busca borrar no es la raíz
		else {
			// Regresas el padre del que acabas de borrar
			Node<T>* parent = root -> remove(val);
			// Mandas a llamar el splay con ese nodo y sobreescribes root
			root = root -> splay(root, parent);
		}
	}
}

template <class T>
bool SplayTree<T>::find(T val){
    // Si no es nulo llamas al find de root (Node)
	if (root != 0) {
		Node <T>* found = root -> find(val);
        root = root -> splay(root, found);
		if (root -> value == val)
			return true;
	} 
	// Si es nulo el árbol es vacío y no jala
	return false;
}

// Borras todo el arbol asegurando que exista
template <class T>
void SplayTree<T>::removeAll(){
	if (root != 0) {
		root -> removeChilds();
	}
	delete root;
	root = 0;
}

template <class T>
string SplayTree<T>::inorder() const{
    stringstream aux;

    aux << "[";
	if (!empty()) {
		root->inorder(aux);
	}
	aux << "]";
	return aux.str();
}

template <class T>
string SplayTree<T>::preorder() const{
    stringstream aux;

    aux << "[";
	if (!empty()) {
		root->preorder(aux);
	}
	aux << "]";
	return aux.str();
}

template <class T>
string SplayTree<T>::print_tree() const{
    stringstream aux;

    if (!empty()) {
		root -> print_tree(aux);
	}

    return aux.str();
}


#endif /* SPLAY_H_ */