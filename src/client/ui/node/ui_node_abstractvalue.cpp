/**
 * @file
 * @brief The abstractvalue node is an abstract node (we can't instanciate it).
 * It provide common properties to concrete nodes, to manage a value in a range.
 */

/*
Copyright (C) 2002-2014 UFO: Alien Invasion.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "../ui_nodes.h"
#include "../ui_parse.h"
#include "../ui_internal.h"
#include "../ui_lua.h"

#include "ui_node_abstractvalue.h"

#include "../../input/cl_input.h"
#include "../../input/cl_keys.h"

#include "../../../common/scripts_lua.h"

#define EXTRADATA_TYPE abstractValueExtraData_t
#define EXTRADATA(node) UI_EXTRADATA(node, EXTRADATA_TYPE)
#define EXTRADATACONST(node) UI_EXTRADATACONST(node, EXTRADATA_TYPE)

/**
 * @brief Allocates a float and initializes it if the pointer value is not set, else does nothing.
 */
static inline void UI_InitCvarOrFloat (float** adress, float defaultValue)
{
	if (*adress == nullptr) {
		*adress = UI_AllocStaticFloat(1);
		**adress = defaultValue;
	}
}

void uiAbstractValueNode::onLoading (uiNode_t* node)
{
	EXTRADATA(node).shiftIncreaseFactor = 2.0F;
}

void uiAbstractValueNode::onLoaded (uiNode_t* node)
{
	UI_InitCvarOrFloat((float**)&EXTRADATA(node).value, 0);
	UI_InitCvarOrFloat((float**)&EXTRADATA(node).delta, 1);
	UI_InitCvarOrFloat((float**)&EXTRADATA(node).max, 0);
	UI_InitCvarOrFloat((float**)&EXTRADATA(node).min, 0);
}

void uiAbstractValueNode::initNodeDynamic (uiNode_t* node)
{
	EXTRADATA(node).value = Mem_PoolAllocType(float, ui_dynPool);
	EXTRADATA(node).delta = Mem_PoolAllocType(float, ui_dynPool);
	EXTRADATA(node).max   = Mem_PoolAllocType(float, ui_dynPool);
	EXTRADATA(node).min   = Mem_PoolAllocType(float, ui_dynPool);
}

void uiAbstractValueNode::deleteNode (uiNode_t* node)
{
	uiNode::deleteNode(node);
	Mem_Free(EXTRADATA(node).value);
	Mem_Free(EXTRADATA(node).delta);
	Mem_Free(EXTRADATA(node).max);
	Mem_Free(EXTRADATA(node).min);
	EXTRADATA(node).value = nullptr;
	EXTRADATA(node).delta = nullptr;
	EXTRADATA(node).max = nullptr;
	EXTRADATA(node).min = nullptr;
}

static void UI_CloneCvarOrFloat (const uiNode_t* source, uiNode_t* clone, const float*const* sourceData, float** cloneData)
{
	/* dont update cvar */
	if (Q_strstart(*(const char*const*)sourceData, "*cvar:")) {
		/* thats anyway a const string */
		assert(!source->dynamic);
		if (clone->dynamic)
			Mem_Free(*(char**)cloneData);
		*(const char**)cloneData = *(const char*const*)sourceData;
	} else {
		/* clone float */
		if (!clone->dynamic)
			*cloneData = UI_AllocStaticFloat(1);
		**cloneData = **sourceData;
	}
}

float uiAbstractValueNode::getFactorFloat (const uiNode_t* node)
{
	if (!Key_IsDown(K_SHIFT))
		return 1.0F;

	return EXTRADATACONST(node).shiftIncreaseFactor;
}

void uiAbstractValueNode::setRange(uiNode_t* node, float min, float max)
{
	if (EXTRADATA(node).min == nullptr) {
		UI_InitCvarOrFloat((float**)&EXTRADATA(node).min, min);
	}
	if (EXTRADATA(node).max == nullptr) {
		UI_InitCvarOrFloat((float**)&EXTRADATA(node).max, max);
	}
}

bool uiAbstractValueNode::setValue(uiNode_t* node, float value)
{
	const float last = UI_GetReferenceFloat(node, EXTRADATA(node).value);
	const float max = UI_GetReferenceFloat(node, EXTRADATA(node).max);
	const float min = UI_GetReferenceFloat(node, EXTRADATA(node).min);

	/* ensure sane values */
	if (value < min)
		value = min;
	else if (value > max)
		value = max;

	/* nothing change? */
	if (last == value) {
		return false;
	}

	/* save result */
	EXTRADATA(node).lastdiff = value - last;
	const char* cvar = Q_strstart((char*)EXTRADATA(node).value, "*cvar:");
	if (cvar)
		Cvar_SetValue(cvar, value);
	else
		*(float*) EXTRADATA(node).value = value;

	/* fire change event */
	if (node->onChange) {
		UI_ExecuteEventActions(node, node->onChange);
	}
	if (node->lua_onChange != LUA_NOREF) {
		UI_ExecuteLuaEventScript(node, node->lua_onChange);
	}

	return true;
}

bool uiAbstractValueNode::setDelta(uiNode_t* node, float delta) {
	const float last = UI_GetReferenceFloat(node, EXTRADATA(node).delta);

	/* nothing change? */
	if (last == delta) {
		return false;
	}

	/* save result */
	const char* cvar = Q_strstart((char*)EXTRADATA(node).delta, "*cvar:");
	if (cvar)
		Cvar_SetValue(cvar, delta);
	else
		*(float*) EXTRADATA(node).delta = delta;

	return true;
}

bool uiAbstractValueNode::incValue(uiNode_t* node)
{
	float value = UI_GetReferenceFloat(node, EXTRADATA(node).value);
	const float delta = getFactorFloat(node) * UI_GetReferenceFloat(node, EXTRADATA(node).delta);
	return setValue(node, value + delta);
}

bool uiAbstractValueNode::decValue(uiNode_t* node)
{
	float value = UI_GetReferenceFloat(node, EXTRADATA(node).value);
	const float delta = getFactorFloat(node) * UI_GetReferenceFloat(node, EXTRADATA(node).delta);
	return setValue(node, value - delta);
}

float uiAbstractValueNode::getMin (uiNode_t const* node)
{
	return UI_GetReferenceFloat(node, EXTRADATACONST(node).min);
}

float uiAbstractValueNode::getMax (uiNode_t const* node)
{
	return UI_GetReferenceFloat(node, EXTRADATACONST(node).max);
}

float uiAbstractValueNode::getDelta (uiNode_t const* node)
{
	return UI_GetReferenceFloat(node, EXTRADATACONST(node).delta);
}

float uiAbstractValueNode::getValue (uiNode_t const* node)
{
	return UI_GetReferenceFloat(node, EXTRADATACONST(node).value);
}

/**
 * @brief Call to update a cloned node
 */
void uiAbstractValueNode::clone (const uiNode_t* source, uiNode_t* clone)
{
	uiLocatedNode::clone(source, clone);
	UI_CloneCvarOrFloat(source, clone, (const float*const*)&EXTRADATACONST(source).value, (float**)&EXTRADATA(clone).value);
	UI_CloneCvarOrFloat(source, clone, (const float*const*)&EXTRADATACONST(source).delta, (float**)&EXTRADATA(clone).delta);
	UI_CloneCvarOrFloat(source, clone, (const float*const*)&EXTRADATACONST(source).max, (float**)&EXTRADATA(clone).max);
	UI_CloneCvarOrFloat(source, clone, (const float*const*)&EXTRADATACONST(source).min, (float**)&EXTRADATA(clone).min);
}

float UI_AbstractValue_GetMin (uiNode_t* node) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	return b->getMin(node);
}

float UI_AbstractValue_GetMax (uiNode_t* node) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	return b->getMax(node);
}

float UI_AbstractValue_GetValue (uiNode_t* node) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	return b->getValue(node);
}

float UI_AbstractValue_GetDelta (uiNode_t* node) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	return b->getDelta(node);
}

void UI_AbstractValue_IncValue (uiNode_t* node) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	b->incValue(node);
}

void UI_AbstractValue_DecValue (uiNode_t* node) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	b->decValue(node);
}

void UI_AbstractValue_SetRange (uiNode_t* node, float min, float max) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	b->setRange(node, min, max);
}

void UI_AbstractValue_SetMin (uiNode_t* node, float min) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	b->setRange(node, min, b->getMax(node));
}

void UI_AbstractValue_SetMax (uiNode_t* node, float max) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	b->setRange(node, b->getMin(node), max);
}

void UI_AbstractValue_SetValue (uiNode_t* node, float value) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	b->setValue(node, value);
}

void UI_AbstractValue_SetDelta (uiNode_t* node, float delta) {
	uiAbstractValueNode* b=static_cast<uiAbstractValueNode*>(node->behaviour->manager.get());
	b->setDelta(node, delta);
}

void UI_AbstractValue_SetRangeCvar (uiNode_t* node, const char* min, const char* max) {
	/* This is a special case: we have a situation where the node already has a (min,max) value
	   (either being floats or cvars). We now want to replace this value by a new cvar. So we first
	   need to free the existing references, then create new cvar references and store them. */
	Mem_Free(*(void**)EXTRADATA(node).min);
	*(void**)EXTRADATA(node).min = Mem_StrDup(min);
	Mem_Free(*(void**)EXTRADATA(node).max);
	*(void**)EXTRADATA(node).max = Mem_StrDup(max);
}

void UI_AbstractValue_SetMinCvar (uiNode_t* node, const char* min) {
	/* This is a special case: we have a situation where the node already has a (min,max) value
	   (either being floats or cvars). We now want to replace this value by a new cvar. So we first
	   need to free the existing references, then create new cvar references and store them. */
	Mem_Free(*(void**)EXTRADATA(node).min);
	*(void**)EXTRADATA(node).min = Mem_StrDup(min);
}

void UI_AbstractValue_SetMaxCvar (uiNode_t* node, const char* max) {
	/* This is a special case: we have a situation where the node already has a (min,max) value
	   (either being floats or cvars). We now want to replace this value by a new cvar. So we first
	   need to free the existing references, then create new cvar references and store them. */
	Mem_Free(*(void**)EXTRADATA(node).max);
	*(void**)EXTRADATA(node).max = Mem_StrDup(max);
}

void UI_AbstractValue_SetValueCvar (uiNode_t* node, const char* value) {
	/* This is a special case: we have a situation where the node already has a value reference
	   (either being float or cvar). We now want to replace this value reference by a new cvar. So we first
	   need to free the existing reference, then create new cvar reference (just a string starting with
	   '*cvar' and store it. */
	Mem_Free(*(void**)(EXTRADATA(node).value));
	*(void**)EXTRADATA(node).value= Mem_StrDup(value);

	/* fire change event */
	if (node->onChange) {
		UI_ExecuteEventActions(node, node->onChange);
	}
	if (node->lua_onChange != LUA_NOREF) {
		UI_ExecuteLuaEventScript(node, node->lua_onChange);
	}
}


void UI_RegisterAbstractValueNode (uiBehaviour_t* behaviour)
{
	behaviour->name = "abstractvalue";
	behaviour->manager = UINodePtr(new uiAbstractValueNode());
	behaviour->isAbstract = true;
	behaviour->extraDataSize = sizeof(EXTRADATA_TYPE);
	behaviour->lua_SWIG_typeinfo = UI_SWIG_TypeQuery("uiAbstractValueNode_t *");

	/* Current value of the node. It should be a cvar */
	UI_RegisterExtradataNodeProperty(behaviour, "current", V_CVAR_OR_FLOAT, abstractValueExtraData_t, value);
	/* Value of a positive step. Must be bigger than 1. */
	UI_RegisterExtradataNodeProperty(behaviour, "delta", V_CVAR_OR_FLOAT, abstractValueExtraData_t, delta);
	/* Maximum value we can set to the node. It can be a cvar. Default value is 0. */
	UI_RegisterExtradataNodeProperty(behaviour, "max", V_CVAR_OR_FLOAT, abstractValueExtraData_t, max);
	/* Minimum value we can set to the node. It can be a cvar. Default value is 1. */
	UI_RegisterExtradataNodeProperty(behaviour, "min", V_CVAR_OR_FLOAT, abstractValueExtraData_t, min);
	/* Defines a factor that is applied to the delta value when the shift key is held down. */
	UI_RegisterExtradataNodeProperty(behaviour, "shiftincreasefactor", V_FLOAT, abstractValueExtraData_t, shiftIncreaseFactor);

	/* Callback value set when before calling onChange. It is used to know the change apply by the user
	 * @Deprecated
	 */
	UI_RegisterExtradataNodeProperty(behaviour, "lastdiff", V_FLOAT, abstractValueExtraData_t, lastdiff);

}
