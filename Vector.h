#ifndef __INC_NFVECTOR_H
#define __INC_NFVECTOR_H


// Minimal class to replace std::vector
template<typename Data>
class Vector {
   size_t d_size; // Stores no. of actually stored objects
   size_t d_capacity; // Stores allocated capacity
   Data *d_data; // Stores data
   public:
    Vector()  // Default constructor
     	: d_size(0), d_capacity(0), d_data(0) {};


    Vector(Vector const &other)  // Copy constuctor
     	: d_size(other.d_size), d_capacity(other.d_capacity), d_data(0) 
    { 
     	d_data = (Data *)malloc(d_capacity*sizeof(Data)); 
     	memcpy(d_data, other.d_data, d_size*sizeof(Data)); 
    };


    ~Vector()  // Destructor
    { 
     	free(d_data); 
    };

    Vector &operator=(Vector const &other)  // Needed for memory management
    { 
     	free(d_data); 
     	d_size = other.d_size; 
     	d_capacity = other.d_capacity; 
     	d_data = (Data *)malloc(d_capacity*sizeof(Data)); 
     	memcpy(d_data, other.d_data, d_size*sizeof(Data)); 
     	return *this; 
    };

    void push_back(Data const &x)  // Adds new value. If needed, allocates more space
    { 
     	if (d_capacity == d_size) 
     	{
     		resize();
     	}
     	d_data[d_size++] = x;
	};

    size_t size() const  // Size getter
    { 
    	return d_size;
    };

    Data const &operator[](size_t idx) const // Const getter
    { 
    	return d_data[idx]; 
    }; 

    Data &operator[](size_t idx)  // Changeable getter
    { 
    	return d_data[idx]; 
    };

   private:
    void resize() // Allocates double the old space
    { 
		d_capacity = d_capacity ? d_capacity*2 : 1;
		Data *newdata = (Data *)malloc(d_capacity*sizeof(Data)); 
		memcpy(newdata, d_data, d_size * sizeof(Data)); 
		free(d_data); d_data = newdata; 
	};
};


#endif