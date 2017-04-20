/* Copyright 2016 Pasu Chan <cschanaj@connect.ust.hk>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* C++ Wrapper for Google Gumbo Parser
 * 
 * First Released August 2016
 */

#include <gumbo.h>

#include <vector>
#include <iostream>

#ifndef __GUMBO_CPP__
#define __GUMBO_CPP__

namespace GumboCpp
{

/* Children */
size_t 
getChildNodesCount(const GumboNode *node);

const GumboNode **
getChildNodesBegin(const GumboNode *node);

const GumboNode **
getChildNodesEnd(const GumboNode *node);

const GumboNode *
getChildNodeFront(const GumboNode *node);

const GumboNode *
getChildNodeBack(const GumboNode *node);


/* Parent */
const GumboNode *
getParentNode(const GumboNode *node);

bool
hasParentNodeOf(const GumboNode *n1, const GumboNode *n2);

bool
hasParentNodeEqual(const GumboNode *node, const GumboNode *p);


/* Sibling */
bool
hasPrevSiblings(const GumboNode *node);

bool
hasNextSiblings(const GumboNode *node);

const GumboNode *
getPrevSibling(const GumboNode *node);

const GumboNode *
getNextSibling(const GumboNode *node);


/* Text */
bool
hasText(const GumboNode *node);

bool
hasTextEqual(const GumboNode *node, const std::string &text);

std::string
getText(const GumboNode *node);


/* Attribute */
size_t 
getAttributesCount(const GumboNode *node);

const GumboAttribute **
getAttributesBegin(const GumboNode *node);

const GumboAttribute **
getAttributesEnd(const GumboNode *node);

bool
hasAttributeName(const GumboNode *node, const std::string &name);

bool
hasAttributeEqual(const GumboNode *node, const std::string &name, 
    const std::string &value);

std::string
getAttributeValue(const GumboNode *node, const std::string &name);


/* Tag */
bool
hasNodeTag(const GumboNode *node);

bool
hasNodeTagEqual(const GumboNode *node, GumboTag tag);

GumboTag
getNodeTag(const GumboNode *node);


/* Algorithm */
template<class UnaryPredicate> const GumboNode *
find_first_if(const GumboNode *node, UnaryPredicate pred)
{
    if(pred(node))
    {
        return node;
    }

    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        if(node = find_first_if(*it++, pred))
        {
            return node;
        }
    }
    return 0;
}

template<class UnaryPredicate> const GumboNode *
find_last_if(const GumboNode *node, UnaryPredicate pred)
{
    const GumboNode *ret = 0;
    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        if(ret = find_last_if(*(--lt), pred))
        {
            return ret;
        }
    }
    return pred(node) ? node : 0;
}

template<class UnaryPredicate> std::vector<const GumboNode *> 
find_if(const GumboNode *node, UnaryPredicate pred)
{
    std::vector<const GumboNode *> ret;

    if(pred(node))
    {
        ret.push_back(node);
    }

    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        std::vector<const GumboNode *> sub = find_if(*it++, pred);
        ret.insert(ret.end(), sub.begin(), sub.end());
    }
    return ret;
}

template<class UnaryPredicate> std::vector<const GumboNode *> 
find_if_not(const GumboNode *node, UnaryPredicate pred)
{
    std::vector<const GumboNode *> ret;

    if(!pred(node))
    {
        ret.push_back(node);
    }

    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        std::vector<const GumboNode *> sub = find_if_not(*it++, pred);
        ret.insert(ret.end(), sub.begin(), sub.end());
    }
    return ret;
}

template<class UnaryPredicate> size_t 
count_if(const GumboNode *node, UnaryPredicate pred)
{
    size_t ret = pred(node) ? 1 : 0;

    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        ret += count_if(*it++, pred);
    }
    return ret;
}

template<class UnaryPredicate> size_t
count_if_not(const GumboNode *node, UnaryPredicate pred)
{
    size_t ret = pred(node) ? 0 : 1;

    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        ret += count_if(*it++, pred);
    }
    return ret;
}

template<class UnaryPredicate> void
for_each(const GumboNode *node, UnaryPredicate pred)
{
    pred(node);

    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        for_each(*it++, pred);
    }
}

template<class UnaryPredicate> void
for_each_reverse(const GumboNode *node, UnaryPredicate pred)
{
    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = getChildNodesEnd(node);

    while(it != lt)
    {
        for_each_reverse(*(--lt), pred);
    }

    pred(node);
}

};

#endif
