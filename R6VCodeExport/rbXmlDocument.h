// ============================================================================
//  rbXmlDocument.h
// ============================================================================
#pragma once



// ============================================================================
// unXmlNode
// ============================================================================
typedef wxXmlNode unXmlNode;


// ============================================================================
// unXmlProperty
// ============================================================================
typedef wxXmlProperty unXmlProperty;


// ============================================================================
// wxXmlNode
// ============================================================================
namespace rbxml
{
	inline wxXmlNode* FindChildNode( wxXmlNode* parent, wxXmlNodeType type=wxXML_ELEMENT_NODE, const wxString& name=wxEmptyString, const wxString& text=wxEmptyString )
	{
		for( wxXmlNode* n=parent->GetChildren(); n!=NULL; n=n->GetNext() )
		{
			if( n->GetType() == type )
			{
				if( name == wxEmptyString || name.IsSameAs(n->GetName()) )
				{
					if( text == wxEmptyString )
					{
						return n;
					}
					else
					{
						if( type == wxXML_TEXT_NODE )
						{
							if( text.IsSameAs(n->GetContent()) )
								return n;
						}
						else
							return FindChildNode(n,wxXML_TEXT_NODE,wxEmptyString,text);
					}
				}
			}
		}
		return NULL;
	}


	inline wxXmlNode* FindChildElement( wxXmlNode* parent, const wxString& name=wxEmptyString, const wxString& text=wxEmptyString )
	{
		return FindChildNode(parent,wxXML_ELEMENT_NODE,name,text);
	}

	inline wxXmlNode* FindChildText( wxXmlNode* parent, const wxString& name=wxEmptyString, const wxString& text=wxEmptyString )
	{
		return FindChildNode(parent,wxXML_TEXT_NODE,name,text);
	}

	inline wxXmlNode* CreateElement( wxXmlNode* parent, const wxString& name, const wxString& text=wxEmptyString )
	{
		if( parent )
		{
			wxXmlNode* node = new wxXmlNode(parent,wxXML_ELEMENT_NODE,name);
			if( node )
			{
				if( text == wxEmptyString )
				{
					return node;
				}
				else
				{
					wxXmlNode* textnode = new wxXmlNode(node,wxXML_TEXT_NODE,wxEmptyString,text);
					if( textnode )
					{
						return node;
					}
					else
					{
						parent->RemoveChild(node);
						return NULL;
					}
				}
			}
		}
		return NULL;
	}

    inline wxXmlNode* GetElement( wxXmlNode* parent, const wxString& name=wxEmptyString, const wxString& text=wxEmptyString, bool bCreate=true )
    {
		if( parent )
		{
			wxXmlNode* node = FindChildElement(parent,name,text);
			if( node )
			{
				return node;
			}
			else if( bCreate )
			{	
				return CreateElement(parent,name,text);
			}
		}
        return NULL;
    }


	// ========================================================================
#if 0
    inline bool FindNodeNamed( wxXmlNode*& node, const wxString& name )
    {
        for( ; node!=NULL; node=node->GetNext() )
            if( node->GetName().IsSameAs( name ) )
                return true;
        return false;
    }

    inline wxString GetNodeText( wxXmlNode* node )
    {
        node = node->GetChildren();
        if( node )   return node->GetContent();
        else         return wxEmptyString;
    }

    inline wxString GetNodeName( wxXmlNode* node )
    {
        if( node )   return node->GetName();
        else         return wxEmptyString;
    }

    inline bool IsSameText( wxXmlNode* node, const wxString& text )
    {
        if( node )  return text.IsSameAs( GetNodeText(node) );
        else        return false;
    }

    inline bool IsSameName( wxXmlNode* node, const wxString& text )
    {
        if( node )  return text.IsSameAs( node->GetName() );
        else        return false;
    }

    inline wxXmlNode* GetNodeNamed( wxXmlNode* node, const wxString& name )
    {
        if( node != NULL && name.IsSameAs( GetNodeName(node) ) )   
            return node;
        else    return NULL;
    }


    inline bool IsElementNode( unXmlNode* node )
    {
        return node != NULL && node->GetType() == wxXML_ELEMENT_NODE;
    }

    inline dword GetElementCount( unXmlNode* parent,  unXmlNode* marker=NULL )
    {
        dword count = 0;
        for( unXmlNode* it=parent->GetChildren(); it!=NULL; it=it->GetNext() )
        {
            if( it == marker )
                break;

            if( IsElementNode(it) && it->GetName() == marker->GetName() )
                ++count;
        }
        return count;
    }

    inline wxString TraceBack( unXmlNode* node )
    {
        wxString s;
        if( IsElementNode(node) )
        {
            for( unXmlNode* parent=node->GetParent(); parent!=NULL; parent=parent->GetParent() )
            {
                if( IsElementNode(parent) )
                {
                    dword count = GetElementCount(parent,node)+1;
                    s.Prepend( wxString::Format( wxT("<%s>(%d)"), node->GetName().c_str(), count ) );
                    node = parent;
                }
            }
            if( node->GetParent() == NULL )
                s.Prepend( wxString::Format( wxT("<%s>"), node->GetName().c_str() ) );
        }
        return s;
    }



    inline wxString GetPropName( wxXmlProperty* prop )
    {
        if( prop )   return prop->GetName();
        else         return wxEmptyString;
    }

    inline wxString GetPropText( wxXmlProperty* prop )
    {
        if( prop )   return prop->GetValue();
        else         return wxEmptyString;
    }

    inline bool IsSameName( wxXmlProperty* prop, const wxString& text )
    {
        if( prop )  return text.IsSameAs( prop->GetName() );
        else        return false;
    }

    inline wxString TraceBack( unXmlNode* node, wxXmlProperty* pnode )
    {
        wxString s;
        if( pnode )
        {
            s = TraceBack(node);
            s.Append( wxString::Format( wxT("@%s"), pnode->GetName().c_str() ) );
        }
        return s;
    }
#endif
}


// ============================================================================
// unXmlDocument
// ============================================================================
class unXmlDocument : public wxXmlDocument
{
public:

};



// ============================================================================
//  EOF
// ============================================================================