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

#include "GumboCpp.h"

/* Children */
size_t
GumboCpp::getChildNodesCount(const GumboNode *node)
{
    if(node && node->type == GUMBO_NODE_ELEMENT)
    {
        return node->v.element.children.length;
    }
    return 0;
}

const GumboNode **
GumboCpp::getChildNodesBegin(const GumboNode *node)
{
    if(getChildNodesCount(node))
    {
        return (const GumboNode **) node->v.element.children.data;
    }
    return 0;
}

const GumboNode **
GumboCpp::getChildNodesEnd(const GumboNode *node)
{
    const GumboNode **it = getChildNodesBegin(node);
    return it ? it + node->v.element.children.length : 0;
}

const GumboNode *
GumboCpp::getChildNodeFront(const GumboNode *node)
{
    const GumboNode **it = getChildNodesBegin(node);
    return it ? *it : 0;
}

const GumboNode *
GumboCpp::getChildNodeBack(const GumboNode *node)
{
    const GumboNode **lt = getChildNodesEnd(node);
    return lt ? *(--lt) : 0;
}


/* Parent */
const GumboNode *
GumboCpp::getParentNode(const GumboNode *node)
{
    return node ? node->parent : 0;
}

bool
GumboCpp::hasParentNodeOf(const GumboNode *n1, const GumboNode *n2)
{
    const GumboNode *p = getParentNode(n1);
    return p && p == getParentNode(n2);
}

bool
GumboCpp::hasParentNodeEqual(const GumboNode *node, const GumboNode *p)
{
    return p && p == getParentNode(node);
}


/* Sibling */
bool
GumboCpp::hasPrevSiblings(const GumboNode *node)
{
    const GumboNode **rt = node ? getChildNodesBegin(node->parent) : 0;
    return rt && *rt != node;
}

bool
GumboCpp::hasNextSiblings(const GumboNode *node)
{
    const GumboNode **rt = node ? getChildNodesEnd(node->parent) : 0;
    return rt && *(--rt) != node;
}

const GumboNode *
GumboCpp::getPrevSibling(const GumboNode *node)
{
    const GumboNode **it = node ? getChildNodesBegin(node->parent) : 0;
    if(it)
    {
        while(*it != node)
        {
            if(*(++it) == node)
            {
                --it;
                return *it;
            }
        }
    }
    return 0;
}

const GumboNode *
GumboCpp::getNextSibling(const GumboNode *node)
{
    const GumboNode **lt = node ? getChildNodesEnd(node->parent) : 0;
    if(lt)
    {
        for(--lt ; *lt != node; )
        {
            if(*(--lt) == node)
            {
                ++lt;
                return *lt;
            }
        }
    }
    return 0;
}

bool
GumboCpp::hasText(const GumboNode *node)
{
    if(node && node->type == GUMBO_NODE_TEXT)
    {
        return true;
    }

    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = it ? it + node->v.element.children.length : 0;

    while(it != lt)
    {
        if(hasText(*it++))
        {
            return true;
        }
    }
    return false;
}

bool
GumboCpp::hasTextEqual(const GumboNode *node, const std::string &text)
{
    return getText(node) == text;
}

std::string 
GumboCpp::getText(const GumboNode *node)
{
    if(node && node->type == GUMBO_NODE_TEXT)
    {
        return node->v.text.text;
    }

    std::string retval;
    const GumboNode **it = getChildNodesBegin(node);
    const GumboNode **lt = it ? it + node->v.element.children.length : 0;

    while(it != lt)
    {
        retval += getText(*it++);
    }
    return retval;
}


/* Attribute */
size_t
GumboCpp::getAttributesCount(const GumboNode *node)
{
    if(node && node->type == GUMBO_NODE_ELEMENT)
    {
        return node->v.element.attributes.length;
    }
    return 0;
}

const GumboAttribute **
GumboCpp::getAttributesBegin(const GumboNode *node)
{
    if(getAttributesCount(node))
    {
        return (const GumboAttribute **) node->v.element.attributes.data;
    }
    return 0;
}

const GumboAttribute **
GumboCpp::getAttributesEnd(const GumboNode *node)
{
    const GumboAttribute **it = getAttributesBegin(node);
    return it ? it + node->v.element.attributes.length : 0;
}

bool
GumboCpp::hasAttributeName(const GumboNode *node, const std::string &name)
{
    if(getAttributesCount(node))
    {
        return gumbo_get_attribute(&node->v.element.attributes, name.c_str());
    }
    return false;
}

bool
GumboCpp::hasAttributeEqual(const GumboNode *node, const std::string &name,
    const std::string &value)
{
    if(getAttributesCount(node))
    {
        const GumboAttribute *attr = gumbo_get_attribute(&node->v.element.attributes, 
            name.c_str());
        return attr && value == attr->value;
    }
    return false;
}

std::string
GumboCpp::getAttributeValue(const GumboNode *node, const std::string &name)
{
    if(getAttributesCount(node))
    {
        const GumboAttribute *attr = gumbo_get_attribute(&node->v.element.attributes, 
            name.c_str());
        return attr ? attr->value : std::string();
    }
    return std::string();
}


/* Tag */
bool
GumboCpp::hasNodeTag(const GumboNode *node)
{
    return node && node->type == GUMBO_NODE_ELEMENT;
}

bool
GumboCpp::hasNodeTagEqual(const GumboNode *node, GumboTag tag)
{
    return hasNodeTag(node) && node->v.element.tag == tag;
}

GumboTag
GumboCpp::getNodeTag(const GumboNode *node)
{
    return hasNodeTag(node) ? node->v.element.tag : GUMBO_TAG_UNKNOWN;
}

