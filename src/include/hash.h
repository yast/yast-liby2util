// from PHI
#ifndef _PkgDb_hash_h                                        /* -*- C++ -*- */
#define _PkgDb_hash_h

#include <list>
#include <vector>
#include <string>

using namespace std;

const size_t primes[] = { 31, 101, 503, 1009, 2003, 5003 };

inline size_t hashfun( unsigned p ) { return size_t(p); };
size_t hashfun( const string& s );
size_t hashfun( string& s );
size_t hashfun( const char * p );

template<class Key>
struct basicHashElt {
	basicHashElt *next;
	Key key;
	basicHashElt(const Key& k) : next(0), key(k) {};
};

template<class Key, class list_elem>
class basic_hash {
public:
	typedef size_t size_type;
	typedef list_elem list_type;
	typedef vector<list_elem*> vector_type;
	typedef size_type (*hashfun_t)( const Key& );

	template<class le>
	class hash_iterator {
		friend class basic_hash<Key, list_elem>;
		typedef forward_iterator_tag iterator_category;
	  private:
		le *current;
		size_type adr;
		const vector_type *vec;

	  public:
		hash_iterator() : vec(0) {}
		hash_iterator( le *c, size_type a, const vector_type *v )
			: current(c), adr(a), vec(v) {}
		// make it possible to convert an iterator to a const_iterator
		hash_iterator( const hash_iterator<list_elem>& i2 ) {
			const hash_iterator& i3 = (const hash_iterator&)i2;
			current = i3.current;
			adr = i3.adr;
			vec = i3.vec;
		}

		// The following operators allow to test a hash in a condition if the
		// iterator is defined
		operator const void* () const { return vec; }	
		bool operator! () const { return vec == 0; }

		le& operator* () const {
			assert(vec);
			return *current;
		}
		le* operator-> () const {
			assert(vec);
			return &*current;
		}

	   hash_iterator& operator++ () {
		   current = (le *)current->next;
		   // If -- after stepping forth in the list -- we've reached the end,
		   // we have to go to the next bucket.
		   if (!current) {
			   while (++adr < vec->size())
				   if ((current = (*vec)[adr]))
					   break;
			   if (adr == vec->size())
				   // end of vector reached
				   vec = 0;
		   }
		   return *this;
	   }
	   hash_iterator operator++ (int) {
		   iterator temp = *this;
		   operator++();
		   return temp;
	   }

	   bool operator== ( const hash_iterator& x ) const {
		   return (!vec && !x.vec) ||
				  (vec && x.vec && current == x.current);
	   }

	   bool operator!= ( const hash_iterator& x ) const {
		  return !operator==(x);
	   }
	}; // iterator

	typedef hash_iterator<list_elem> iterator;
	typedef hash_iterator<const list_elem> const_iterator;
	friend class iterator;
	friend class const_iterator;

protected:
	vector_type v;
	size_type vsize;      // # of buckets (vector size)
	size_type n_elements; // # of stored elements
	size_type n_buckets;  // # of used buckets
	hashfun_t hf;
private:

	// helper called from copy constructor and operator=
	void construct( const basic_hash& S ) {
		hf = S.hf;
		vsize = S.vsize;
		v = vector_type( vsize, 0 );
		n_elements = 0;
		n_buckets = 0;
		for( iterator t = S.begin(); t != S.end(); ++t )
			insert( t );
	}

	void resize( size_type new_size ) {
		basic_hash newhash(new_size, hf);
		for( iterator t = begin(); t != end(); ++t )
			newhash.insert( t );
		swap( newhash );
	}

	size_type next_size() {
		unsigned i;
		for( i = 0; i < sizeof(primes)/sizeof(*primes)-1; ++i ) {
			if (primes[i] > vsize)
				break;
		}
		return primes[i];
	}

	bool resize_if_needed() {
		// resize hash if average list length exceeds 1.8 = 9/5
		if (5*n_elements >= 9*n_buckets) {
			resize( next_size() );
			return true;
		}
		return false;
	}
	
  public:
	basic_hash( size_type size = 31, hashfun_t f = hashfun ) :
		v(size, 0), vsize(size), n_elements(0), n_buckets(0), hf(f) {}
	basic_hash( const basic_hash& S ) { construct(S); }
	~basic_hash() { clear(); }
	basic_hash& operator= ( const basic_hash& S ) {
		if (this != &S) {
			clear();
			construct(S);
		}
		return *this;
	}

	iterator begin() {
		size_type adr = 0;
		while( adr < vsize ) {
			if (!v[adr])
				++adr;
			else
				return iterator(v[adr], adr, &v);
		}
		return iterator();
	}
	iterator end() { return iterator(); }
	const_iterator const_begin() const {
		size_type adr = 0;
		while( adr < vsize ) {
			if (!v[adr])
				++adr;
			else
				return const_iterator(v[adr], adr, &v);
		}
		return const_iterator();
	}
	const_iterator begin() const { return const_begin(); }
	const_iterator const_end() const { return const_iterator(); }
	const_iterator end() const { return const_end(); }

	// destruct all contents
	void clear() {
		for( size_type i = 0; i < vsize; i++ ) {
			if (list_elem *p = v[i]) {
				list_elem *pnext;
				while( p ) {
					pnext = (list_elem *)p->next;
					delete p;
					p = pnext;
				}
				v[i] = 0;
			}
		}
		n_elements = 0;
		n_buckets = 0;
	}
	
	list_elem *find( const Key& k, list_elem ***where = 0 ) {
		size_type adr = hf(k) % vsize;
		list_elem **p = &v[adr];
		if (!*p) {
			if (where) {
				*where = p;
				++n_buckets;
			}
			return 0;
		}
		for( ; *p; p = (list_elem **)&((*p)->next) ) {
			if ((*p)->key == k)
				return *p;
		}
		if (where) *where = p;
		return 0;
	}

	const list_elem *find( const Key& k ) const {
		return (const_cast<basic_hash<Key,list_elem>*>(this)->find(k));
	}
	
	bool exists( const Key& k ) const {
		return (const_cast<basic_hash<Key,list_elem>*>(this)->find(k) != 0);
	};
	
	// insert methods for compatibility with STL
	list_elem* insert( const Key& k ) {
		list_elem **p;
		list_elem *q;
		if ((q = find( k, &p )))
			return q;
		else {
			*p = new list_elem(k);
			++n_elements;
			return *p;
		}
	}
	list_elem* insert( const list_elem *e ) { return insert( e->k ); }
	list_elem* insert( const iterator& i )  { return insert( i->k ); }
	
	bool erase( const Key& k ) {
		size_type adr = hf(k) % vsize;
		list_elem **p = &v[adr];
		if (!*p)
			return false;
		for( ; *p; p = (list_elem **)&((*p)->next) ) {
			if ((*p)->key == k) {
				list_elem *q = *p;
				*p = (list_elem *)q->next;
				delete q;
				--n_elements;
				if (!*p) --n_buckets;
				return true;
			}
		}
		return false;
	}
	bool erase( const iterator& i ) { return erase( i->k ); }

	size_type size()	 const { return n_elements; }
	size_type max_size() const { return vsize; }
	bool empty()		 const { return n_elements == 0; }
	void swap( basic_hash& s ) {
	   v.swap(s.v);
	   std::swap(n_elements, s.n_elements);
	   std::swap(n_buckets, s.n_buckets);
	   std::swap(hf, s.hf);
	   std::swap(vsize, s.vsize);
	}

	void statistics();
};


template<class Key>
class noval_hash : public basic_hash<Key,basicHashElt<Key> > {};

template<class Key, class T>
struct HashElt : public basicHashElt<Key> {
	T value;
	HashElt(const Key& k, const T& v) : basicHashElt<Key>(k), value(v) {};
};

template<class Key, class T>
class hash : public basic_hash<Key,HashElt<Key,T> > {
	typedef basicHashElt<Key> list_elem;
	typedef HashElt<Key,T> list_type;

	// helper called from copy constructor and operator=
	void construct( const hash& S ) {
		hf = S.hf;
		vsize = S.vsize;
		v = vector_type( vsize, 0 );
		n_elements = 0;
		n_buckets = 0;
		for( typename basic_hash<Key,HashElt<Key,T> >::const_iterator t = S.begin(); t != S.end(); ++t )
			insert( t );
	}

	void resize( typename basic_hash<Key,HashElt<Key,T> >::size_type new_size ) {
		hash newhash(new_size, hf);
		for( typename basic_hash<Key,HashElt<Key,T> >::const_iterator t = begin(); t != end(); ++t )
			newhash.insert( t );
		swap( newhash );
	}

public:
	hash( typename basic_hash<Key,HashElt<Key,T> >::size_type size = 31, typename basic_hash<Key,HashElt<Key,T> >::hashfun_t f = hashfun ) :
		basic_hash<Key,HashElt<Key,T> >( size, hashfun ) {};
	hash( const hash& S ) { construct(S); }
	~hash() { clear(); }
	hash& operator= ( const hash& S ) {
		if (this != &S) {
			clear();
			construct(S);
		}
		return *this;
	}

	T& operator[] ( const Key& k ) {
		return insert( k, T() )->value;
	}

	T operator[] ( const Key& k ) const {
		const list_type *p = find(k);
		return p ? p->value : T();
	}

	list_type* insert( const Key& k, const T& v ) {
		list_type *q;
		list_type **p;
		if ((q = find( k, &p )))
			return q;
		else {
			*p = new list_type(k,v);
			++n_elements;
			return *p;
		}
	}
	list_type* insert( const list_type *e ) {
		return insert( e->key, e->value ); }
	list_type* insert( const typename hash<Key,T>::iterator& i ) {
		return insert( i->key, i->value ); }
	list_type* insert( const typename hash<Key,T>::const_iterator& i ) {
		return insert( i->key, i->value ); }
};

template <class Key, class list_elem>
void basic_hash<Key, list_elem>::statistics() {
	cout << n_elements << " elements in " << n_buckets << "/" << vsize
		 << " buckets\n";
	unsigned free = 0, alloced = 0, maxlen = 0, sumlen = 0;
	size_type maxbuck;
	
	for( size_type adr = 0; adr < vsize; ++adr ) {
		if (!v[adr])
			++free;
		else {
			++alloced;
			size_type len = 0;
			for( list_elem *p = (list_elem*)v[adr]; p; p = (list_elem*)p->next )
				++len;
			if (len > maxlen) {
				maxlen = len;
				maxbuck = adr;
			}
			sumlen += len;
		}
	}
	cout << free << " buckets free, " << alloced << " used\n";
	cout << "avg. list len " << (float)sumlen/alloced <<
		" max len " << maxlen << endl;

	cout << "Estimated avg. list len " << (float)n_elements/n_buckets << endl;
}

#endif	/* _PkgDb_hash_h */

// Local Variables:
// tab-width: 4
// End:
