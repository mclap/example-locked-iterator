#pragma once

#include <boost/shared_ptr.hpp>

template<typename BaseIterator,typename RhsIterator,typename Guard,typename Lock>
class base_locked_iterator
{
public:
     typedef typename BaseIterator::value_type value_type;
     typedef typename BaseIterator::pointer pointer;
     // here we can use original reference type - because of guard_()
     typedef typename BaseIterator::reference reference;

     // a little bit dangerous to have same iterator_category
     typedef typename BaseIterator::iterator_category iterator_category;
     typedef typename BaseIterator::difference_type difference_type;

     // for conversion purposes
     typedef base_locked_iterator<RhsIterator,BaseIterator,Guard,Lock> c_rhs_iterator;

     base_locked_iterator()
          : guard_()
          , it_()
          , eod_(true)
     {
     }

     base_locked_iterator(BaseIterator it, Lock *lock_, bool eod = false)
     {
          if (lock_)
               guard_.reset(new Guard(*lock_));

          it_ = it;
          eod_ = eod;
     }

     ~base_locked_iterator()
     {
     }

     base_locked_iterator(const base_locked_iterator& v)
     {
          guard_ = v.guard_;
          it_ = v.it_;
          eod_ = v.eod_;
     }

     base_locked_iterator(RhsIterator& it, boost::shared_ptr<Guard>& guard, bool eod)
     {
          guard_ = guard;
          it_ = it;
          eod_ = eod;
     }

     reference operator *()
     {
          return *it_;
     }

     reference operator *() const
     {
          return *it_;
     }

     reference operator ->()
     {
          return *it_;
     }

     reference operator ->() const
     {
          return *it_;
     }

     /// Prefix
     base_locked_iterator operator++()
     {
          ++it_;
          return *this;
     }

     /// Postfix
     base_locked_iterator operator++(int)
     {
          base_locked_iterator copy(*this);
          ++it_;
          return copy;
     }

     bool operator==(const base_locked_iterator& rhs) const
     {
          return !(rhs != *this);
     }

     bool operator!=(const base_locked_iterator& rhs) const
     {
          if (rhs.eod_ == eod_)
               return false;

          return rhs.it_ != it_;
     }

     base_locked_iterator& operator=( const base_locked_iterator& rhs )
     {
          if ( this != &rhs )
          {
               it_ = rhs.it_;
               eod_ = rhs.eod_;
               guard_ = rhs.guard_;
          }

          return *this;
     }

     base_locked_iterator& operator=( const c_rhs_iterator& rhs )
     {
               it_ = rhs.it_;
               eod_ = rhs.eod_;
               guard_ = rhs.guard_;

          return *this;
     }

     operator c_rhs_iterator()
     {
          return c_rhs_iterator(it_, guard_, eod_);
     }

     BaseIterator& GetRawIterator()
     {
          return it_;
     }

private:
     BaseIterator it_;
     boost::shared_ptr<Guard> guard_;
     bool eod_;

};

template<typename Container, typename Lock, typename Guard>
class locked_container
{
private:
public:
     typedef typename Container::iterator ContainerIterator;
     typedef typename Container::const_iterator ContainerConstIterator;
     typedef typename Container::size_type size_type;
     typedef typename Container::value_type value_type;
     // cannot use real reference here
     typedef typename Container::value_type reference;
     typedef const typename Container::value_type const_reference;

     typedef base_locked_iterator<ContainerIterator,ContainerConstIterator,Guard,Lock> iterator;
     typedef base_locked_iterator<ContainerConstIterator,ContainerIterator,Guard,Lock> const_iterator;

     locked_container()
          : data_()
          , lock_()
     { }

     locked_container(const Container& v)
          : data_(v)
          , lock_()
     {
     }

     locked_container(const locked_container& v)
          : data_()
          , lock_()
     {
          Guard guard(v.lock_);
          data_ = v.data_;
     }

     void push_back(const value_type& value)
     {
          Guard guard(lock_);
          data_.push_back(value);
     }

     const value_type front() const
     {
          Guard guard(lock_);
          return data_.front();
     }

     const value_type back() const
     {
          Guard guard(lock_);
          return data_.front();
     }

     void pop_front()
     {
          Guard guard(lock_);
          data_.pop_front();
     }

     void erase( iterator it )
     {
          data_.erase( it.GetRawIterator() );
     }

     void remove( const value_type& v )
     {
          Guard guard(lock_);
          data_.remove( v );
     }

     void sort()
     {
          Guard guard(lock_);
          data_.sort();
     }

     void unique()
     {
          Guard guard(lock_);
          data_.unique();
     }

     iterator begin()
     {
          return iterator(data_.begin(), &lock_);
     }
     const_iterator begin() const
     {
          return const_iterator(data_.begin(), &lock_);
     }

     iterator end()
     {
          return iterator(data_.end(), NULL, true);
     }
     const_iterator end() const
     {
          return const_iterator(data_.end(), NULL, true);
     }

     size_type size() const
     {
          Guard guard(lock_);
          return data_.size();
     }

     bool empty() const
     {
          Guard guard(lock_);
          return data_.empty();
     }

     void clear()
     {
          Guard guard(lock_);
          data_.clear();
     }

     void swap(locked_container& rhs)
     {
          Guard guard(lock_);
          Guard guard_rhs(rhs.lock_);

          data_.swap(rhs.data_);
     }

     locked_container& operator=(const locked_container& rhs)
     {
          if (this != &rhs)
          {
               Guard guard(lock_);
               Guard guard_rhs(rhs.lock_);

               data_ = rhs.data_;
          }

          return *this;
     }


private:
     Container data_;
     mutable Lock lock_;
};
