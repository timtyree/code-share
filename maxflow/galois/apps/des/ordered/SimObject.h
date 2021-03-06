/** SimObject for Ordered algorithm -*- C++ -*-
 * @file
 * @section License
 *
 * Galois, a framework to exploit amorphous data-parallelism in irregular
 * programs.
 *
 * Copyright (C) 2011, The University of Texas at Austin. All rights reserved.
 * UNIVERSITY EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES CONCERNING THIS
 * SOFTWARE AND DOCUMENTATION, INCLUDING ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR ANY PARTICULAR PURPOSE, NON-INFRINGEMENT AND WARRANTIES OF
 * PERFORMANCE, AND ANY WARRANTY THAT MIGHT OTHERWISE ARISE FROM COURSE OF
 * DEALING OR USAGE OF TRADE.  NO WARRANTY IS EITHER EXPRESS OR IMPLIED WITH
 * RESPECT TO THE USE OF THE SOFTWARE OR DOCUMENTATION. Under no circumstances
 * shall University be liable for incidental, special, indirect, direct or
 * consequential damages or loss of profits, interruption of business, or
 * related expenses which may arise from use of Software or Documentation,
 * including but not limited to those resulting from defects in Software and/or
 * Documentation, or loss or inaccuracy of data of any kind.
 *
 * @author M. Amber Hassaan <ahassaan@ices.utexas.edu>
 */

#ifndef ORDERED_SIM_OBJECT_H
#define ORDERED_SIM_OBJECT_H

#include <sstream>
#include <iostream>
#include <vector>

#include "comDefs.h"
#include "BaseSimObject.h"
#include "Event.h"

namespace des_ord {

template <typename Event_tp>
class SimObject: public des::BaseSimObject<Event_tp> {

  typedef des::BaseSimObject<Event_tp> Base;
  typedef Event_tp Event_ty;

  template <typename Cont> 
  struct SendAddList: public Base::SendWrapper {
    Cont& container;

    SendAddList (Cont& _container): Base::SendWrapper (), container (_container) {}

    virtual void send (Base* dstObj, const Event_ty& event) {
      container.push_back (event);
    }
  };


public:

  SimObject (size_t id, unsigned numOutputs, unsigned numInputs)
    : Base (id) 
  {}

  template <typename G, typename C>
  void execEvent (
      const Event_ty& event, 
      G& graph, 
      typename G::GraphNode& mynode, 
      C& newEvents) {

    assert (event.getRecvObj () == this);

    typename Base::template OutDegIterator<G> beg = this->make_begin (graph, mynode);
    typename Base::template OutDegIterator<G> end = this->make_end (graph, mynode);

    SendAddList<C> addListWrap (newEvents);
    this->execEventIntern (event, addListWrap, beg, end);
  }
  
};

} // end namespace des_ord

#endif // ORDERED_SIM_OBJECT_H
