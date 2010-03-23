/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set tw=80 expandtab softtabstop=2 ts=2 sw=2: */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#include "nsIDOMHTMLAreaElement.h"
#include "nsIDOMNSHTMLAreaElement2.h"
#include "nsIDOMEventTarget.h"
#include "nsGenericHTMLElement.h"
#include "nsILink.h"
#include "nsIPresShell.h"
#include "nsGkAtoms.h"
#include "nsStyleConsts.h"
#include "nsPresContext.h"
#include "nsIEventStateManager.h"
#include "nsIURL.h"
#include "nsNetUtil.h"
#include "nsReadableUtils.h"
#include "nsIDocument.h"

class nsHTMLAreaElement : public nsGenericHTMLElement,
                          public nsIDOMHTMLAreaElement,
                          public nsIDOMNSHTMLAreaElement2,
                          public nsILink
{
public:
  nsHTMLAreaElement(nsINodeInfo *aNodeInfo);
  virtual ~nsHTMLAreaElement();

  // nsISupports
  NS_DECL_ISUPPORTS_INHERITED

  // nsIDOMNode
  NS_FORWARD_NSIDOMNODE(nsGenericHTMLElement::)

  // nsIDOMElement
  NS_FORWARD_NSIDOMELEMENT(nsGenericHTMLElement::)

  // nsIDOMHTMLElement
  NS_FORWARD_NSIDOMHTMLELEMENT(nsGenericHTMLElement::)

  // nsIDOMHTMLAreaElement
  NS_DECL_NSIDOMHTMLAREAELEMENT

  // nsIDOMNSHTMLAreaElement
  NS_DECL_NSIDOMNSHTMLAREAELEMENT

  // nsIDOMNSHTMLAreaElement2
  NS_DECL_NSIDOMNSHTMLAREAELEMENT2

  // nsILink
  NS_IMETHOD LinkAdded() { return NS_OK; }
  NS_IMETHOD LinkRemoved() { return NS_OK; }

  virtual nsresult PreHandleEvent(nsEventChainPreVisitor& aVisitor);
  virtual nsresult PostHandleEvent(nsEventChainPostVisitor& aVisitor);
  virtual PRBool IsLink(nsIURI** aURI) const;
  virtual void GetLinkTarget(nsAString& aTarget);
  virtual nsLinkState GetLinkState() const;
  virtual void SetLinkState(nsLinkState aState);
  virtual already_AddRefed<nsIURI> GetHrefURI() const;

  virtual nsresult BindToTree(nsIDocument* aDocument, nsIContent* aParent,
                              nsIContent* aBindingParent,
                              PRBool aCompileEventHandlers);
  virtual void UnbindFromTree(PRBool aDeep = PR_TRUE,
                              PRBool aNullParent = PR_TRUE);
  nsresult SetAttr(PRInt32 aNameSpaceID, nsIAtom* aName,
                   const nsAString& aValue, PRBool aNotify)
  {
    return SetAttr(aNameSpaceID, aName, nsnull, aValue, aNotify);
  }
  virtual nsresult SetAttr(PRInt32 aNameSpaceID, nsIAtom* aName,
                           nsIAtom* aPrefix, const nsAString& aValue,
                           PRBool aNotify);
  virtual nsresult UnsetAttr(PRInt32 aNameSpaceID, nsIAtom* aAttribute,
                             PRBool aNotify);

  virtual nsresult Clone(nsINodeInfo *aNodeInfo, nsINode **aResult) const;

protected:
  // The cached visited state
  nsLinkState mLinkState;
};


NS_IMPL_NS_NEW_HTML_ELEMENT(Area)


nsHTMLAreaElement::nsHTMLAreaElement(nsINodeInfo *aNodeInfo)
  : nsGenericHTMLElement(aNodeInfo),
    mLinkState(eLinkState_Unknown)
{
}

nsHTMLAreaElement::~nsHTMLAreaElement()
{
}

NS_IMPL_ADDREF_INHERITED(nsHTMLAreaElement, nsGenericElement) 
NS_IMPL_RELEASE_INHERITED(nsHTMLAreaElement, nsGenericElement) 


// QueryInterface implementation for nsHTMLAreaElement
NS_INTERFACE_TABLE_HEAD(nsHTMLAreaElement)
  NS_HTML_CONTENT_INTERFACE_TABLE4(nsHTMLAreaElement,
                                   nsIDOMHTMLAreaElement,
                                   nsIDOMNSHTMLAreaElement,
                                   nsIDOMNSHTMLAreaElement2,
                                   nsILink)
  NS_HTML_CONTENT_INTERFACE_TABLE_TO_MAP_SEGUE(nsHTMLAreaElement,
                                               nsGenericHTMLElement)
NS_HTML_CONTENT_INTERFACE_TABLE_TAIL_CLASSINFO(HTMLAreaElement)


NS_IMPL_ELEMENT_CLONE(nsHTMLAreaElement)


NS_IMPL_STRING_ATTR(nsHTMLAreaElement, AccessKey, accesskey)
NS_IMPL_STRING_ATTR(nsHTMLAreaElement, Alt, alt)
NS_IMPL_STRING_ATTR(nsHTMLAreaElement, Coords, coords)
NS_IMPL_URI_ATTR(nsHTMLAreaElement, Href, href)
NS_IMPL_BOOL_ATTR(nsHTMLAreaElement, NoHref, nohref)
NS_IMPL_STRING_ATTR(nsHTMLAreaElement, Shape, shape)
NS_IMPL_INT_ATTR_DEFAULT_VALUE(nsHTMLAreaElement, TabIndex, tabindex, 0)

NS_IMETHODIMP
nsHTMLAreaElement::GetTarget(nsAString& aValue)
{
  if (!GetAttr(kNameSpaceID_None, nsGkAtoms::target, aValue)) {
    GetBaseTarget(aValue);
  }
  return NS_OK;
}

NS_IMETHODIMP
nsHTMLAreaElement::SetTarget(const nsAString& aValue)
{
  return SetAttr(kNameSpaceID_None, nsGkAtoms::target, aValue, PR_TRUE);
}

nsresult
nsHTMLAreaElement::PreHandleEvent(nsEventChainPreVisitor& aVisitor)
{
  return PreHandleEventForAnchors(aVisitor);
}

nsresult
nsHTMLAreaElement::PostHandleEvent(nsEventChainPostVisitor& aVisitor)
{
  return PostHandleEventForAnchors(aVisitor);
}

PRBool
nsHTMLAreaElement::IsLink(nsIURI** aURI) const
{
  return IsHTMLLink(aURI);
}

void
nsHTMLAreaElement::GetLinkTarget(nsAString& aTarget)
{
  GetAttr(kNameSpaceID_None, nsGkAtoms::target, aTarget);
  if (aTarget.IsEmpty()) {
    GetBaseTarget(aTarget);
  }
}

nsresult
nsHTMLAreaElement::BindToTree(nsIDocument* aDocument, nsIContent* aParent,
                              nsIContent* aBindingParent,
                              PRBool aCompileEventHandlers)
{
  nsresult rv = nsGenericHTMLElement::BindToTree(aDocument, aParent,
                                                 aBindingParent,
                                                 aCompileEventHandlers);
  NS_ENSURE_SUCCESS(rv, rv);

  if (aDocument) {
    RegUnRegAccessKey(PR_TRUE);
  }

  return rv;
}

void
nsHTMLAreaElement::UnbindFromTree(PRBool aDeep, PRBool aNullParent)
{
  if (IsInDoc()) {
    RegUnRegAccessKey(PR_FALSE);
    // Wallpaper null check see bug 480300
    nsIDocument* doc = GetCurrentDoc();
    if (doc)
      doc->ForgetLink(this);

    // If this link is ever reinserted into a document, it might
    // be under a different xml:base, so forget the cached state now
    mLinkState = eLinkState_Unknown;
  }

  nsGenericHTMLElement::UnbindFromTree(aDeep, aNullParent);
}

nsresult
nsHTMLAreaElement::SetAttr(PRInt32 aNameSpaceID, nsIAtom* aName,
                           nsIAtom* aPrefix, const nsAString& aValue,
                           PRBool aNotify)
{
  if (aName == nsGkAtoms::accesskey && aNameSpaceID == kNameSpaceID_None) {
    RegUnRegAccessKey(PR_FALSE);
  }

  if (aName == nsGkAtoms::href && aNameSpaceID == kNameSpaceID_None) {
    nsIDocument* doc = GetCurrentDoc();
    if (doc) {
      doc->ForgetLink(this);
      // The change to 'href' will cause style reresolution which will
      // eventually recompute the link state and re-add this element
      // to the link map if necessary.
    }
    SetLinkState(eLinkState_Unknown);
  }

  nsresult rv =
    nsGenericHTMLElement::SetAttr(aNameSpaceID, aName, aPrefix, aValue, aNotify);

  if (aName == nsGkAtoms::accesskey && aNameSpaceID == kNameSpaceID_None &&
      !aValue.IsEmpty()) {
    RegUnRegAccessKey(PR_TRUE);
  }

  return rv;
}

nsresult
nsHTMLAreaElement::UnsetAttr(PRInt32 aNameSpaceID, nsIAtom* aAttribute,
                             PRBool aNotify)
{
  if (aAttribute == nsGkAtoms::href && kNameSpaceID_None == aNameSpaceID) {
    nsIDocument* doc = GetCurrentDoc();
    if (doc) {
      doc->ForgetLink(this);
    }
    SetLinkState(eLinkState_Unknown);
  }

  if (aAttribute == nsGkAtoms::accesskey &&
      aNameSpaceID == kNameSpaceID_None) {
    RegUnRegAccessKey(PR_FALSE);
  }

  return nsGenericHTMLElement::UnsetAttr(aNameSpaceID, aAttribute, aNotify);
}

#define IMPL_URI_PART(_part)                                 \
  NS_IMETHODIMP                                              \
  nsHTMLAreaElement::Get##_part(nsAString& a##_part)         \
  {                                                          \
    return Get##_part##FromHrefURI(a##_part);                \
  }                                                          \
  NS_IMETHODIMP                                              \
  nsHTMLAreaElement::Set##_part(const nsAString& a##_part)   \
  {                                                          \
    return Set##_part##InHrefURI(a##_part);                  \
  }

IMPL_URI_PART(Protocol)
IMPL_URI_PART(Host)
IMPL_URI_PART(Hostname)
IMPL_URI_PART(Pathname)
IMPL_URI_PART(Search)
IMPL_URI_PART(Port)
IMPL_URI_PART(Hash)

#undef IMPL_URI_PART

NS_IMETHODIMP
nsHTMLAreaElement::ToString(nsAString& aSource)
{
  return GetHref(aSource);
}

NS_IMETHODIMP    
nsHTMLAreaElement::GetPing(nsAString& aValue)
{
  return GetURIListAttr(nsGkAtoms::ping, aValue);
}

NS_IMETHODIMP
nsHTMLAreaElement::SetPing(const nsAString& aValue)
{
  return SetAttr(kNameSpaceID_None, nsGkAtoms::ping, aValue, PR_TRUE);
}

nsLinkState
nsHTMLAreaElement::GetLinkState() const
{
  return mLinkState;
}

void
nsHTMLAreaElement::SetLinkState(nsLinkState aState)
{
  mLinkState = aState;
}

already_AddRefed<nsIURI>
nsHTMLAreaElement::GetHrefURI() const
{
  return GetHrefURIForAnchors();
}
