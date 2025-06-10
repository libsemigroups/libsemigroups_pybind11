..
    Copyright (c) 2024 J. D. Mitchell

    Distributed under the terms of the GPL license version 3.

    The full license is in the file LICENSE, distributed with this software.

.. currentmodule:: libsemigroups_pybind11

Obviously infinite
==================

This page collects the documentation for the functionality in
``libsemigroups_pybind11`` for checking if a finitely presented semigroup or
monoid is obviously infinite.

The functions below implement a number of checks for whether or not a finitely
presented semigroup or monoid is infinite. These checks are all decidable, and
always return an answer within an amount of time that is linear in the size of
the input.

These checks are:

    1. For every generator there is at least one side of one relation that
       consists solely of that generator. If this condition is not met, then
       there is a generator of infinite order.
    2. The number of occurrences of every generator is not preserved by the
       relations. Otherwise, it is not possible to use the relations to reduce
       the number of occurrences of a generator in a word, and so there are
       infinitely many distinct words.
    3. The number of generators on the left hand side of a relation is not the
       same as the number of generators on the right hand side for at least one
       generator. Otherwise the relations preserve the length of any word and so
       there are infinitely many distinct words.
    4. There are at least as many relations as there are generators. Otherwise
       we can find a surjective homomorphism onto an infinite subsemigroup of
       the rationals under addition.
    5. The checks 2., 3. and 4. are a special case of a more general matrix
       based condition. We construct a matrix whose columns correspond to
       generators and rows correspond to relations. The ``(i, j)``-th entry is
       the number of occurrences of the ``j``-th generator in the left hand side
       of the ``i``-th relation minus the number of occurrences of it on the
       right hand side. If this matrix has a non-trivial kernel, then we can
       construct a surjective homomorphism onto an infinite subsemigroup of the
       rationals under addition. So we check that the matrix is full rank.
    6. The presentation is not that of a free product. To do this we consider a
       graph whose vertices are generators and an edge connects two generators
       if they occur on either side of the same relation. If this graph is
       disconnected then the presentation is a free product and is therefore
       infinite. Note that we currently do not consider the case where the
       identity occurs in the presentation.

.. autofunction:: is_obviously_infinite
