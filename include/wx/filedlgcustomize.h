///////////////////////////////////////////////////////////////////////////////
// Name:        wx/filedlgcustomize.h
// Purpose:     Classes for wxFileDialog customization.
// Author:      Vadim Zeitlin
// Created:     2022-05-26
// Copyright:   (c) 2022 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FILEDLGCUSTOMIZE_H_
#define _WX_FILEDLGCUSTOMIZE_H_

#include "wx/vector.h"

class wxFileDialogCustomControlImpl;
class wxFileDialogButtonImpl;
class wxFileDialogCheckBoxImpl;
class wxFileDialogTextCtrlImpl;
class wxFileDialogStaticTextImpl;
class wxFileDialogCustomizeImpl;

// ----------------------------------------------------------------------------
// wxFileDialog custom controls
// ----------------------------------------------------------------------------

// All these controls support a very limited set of functions, but use the same
// names for the things that they do support as the corresponding "normal" wx
// classes.

// The base class for all wxFileDialog custom controls.
class wxFileDialogCustomControl
{
public:
    void Show(bool show = true);
    void Hide() { Show(false); }

    void Enable(bool enable = true);
    void Disable() { Enable(false); }

    ~wxFileDialogCustomControl();

protected:
    explicit wxFileDialogCustomControl(wxFileDialogCustomControlImpl* impl)
        : m_impl(impl)
    {
    }

    wxFileDialogCustomControlImpl* const m_impl;

    wxDECLARE_NO_COPY_CLASS(wxFileDialogCustomControl);
};

// A class representing a custom button.
class wxFileDialogButton : public wxFileDialogCustomControl
{
public:
    // Ctor is only used by wxWidgets itself.
    explicit wxFileDialogButton(wxFileDialogButtonImpl* impl);

private:
    wxFileDialogButtonImpl* GetImpl() const;

    wxDECLARE_NO_COPY_CLASS(wxFileDialogButton);
};

// A class representing a custom checkbox.
class wxFileDialogCheckBox : public wxFileDialogCustomControl
{
public:
    bool GetValue() const;
    void SetValue(bool value);

    // Ctor is only used by wxWidgets itself.
    explicit wxFileDialogCheckBox(wxFileDialogCheckBoxImpl* impl);

private:
    wxFileDialogCheckBoxImpl* GetImpl() const;

    wxDECLARE_NO_COPY_CLASS(wxFileDialogCheckBox);
};

// A class representing a custom text control.
class wxFileDialogTextCtrl : public wxFileDialogCustomControl
{
public:
    wxString GetValue() const;
    void SetValue(const wxString& text);

    // Ctor is only used by wxWidgets itself.
    explicit wxFileDialogTextCtrl(wxFileDialogTextCtrlImpl* impl);

private:
    wxFileDialogTextCtrlImpl* GetImpl() const;

    wxDECLARE_NO_COPY_CLASS(wxFileDialogTextCtrl);
};

// A class representing a custom static text.
class wxFileDialogStaticText : public wxFileDialogCustomControl
{
public:
    void SetLabelText(const wxString& text);

    // Ctor is only used by wxWidgets itself.
    explicit wxFileDialogStaticText(wxFileDialogStaticTextImpl* impl);

private:
    wxFileDialogStaticTextImpl* GetImpl() const;

    wxDECLARE_NO_COPY_CLASS(wxFileDialogStaticText);
};

// ----------------------------------------------------------------------------
// wxFileDialogCustomizer is used by wxFileDialogCustomizeHook
// ----------------------------------------------------------------------------

class wxFileDialogCustomize
{
public:
    wxFileDialogButton* AddButton(const wxString& label);
    wxFileDialogCheckBox* AddCheckBox(const wxString& label);
    wxFileDialogTextCtrl* AddTextCtrl();
    wxFileDialogStaticText* AddStaticText(const wxString& label);

    ~wxFileDialogCustomize();

protected:
    // Ctor is only used by wxWidgets itself.
    //
    // Note that we don't take ownership of the implementation pointer here,
    // see the comment in the dtor for more details.
    explicit wxFileDialogCustomize(wxFileDialogCustomizeImpl* impl)
        : m_impl(impl)
    {
    }

private:
    template <typename T> T* StoreAndReturn(T* control);

    wxFileDialogCustomizeImpl* const m_impl;

    wxVector<wxFileDialogCustomControl*> m_controls;

    wxDECLARE_NO_COPY_CLASS(wxFileDialogCustomize);
};

// ----------------------------------------------------------------------------
// wxFileDialogCustomizeHook: used by wxFileDialog itself
// ----------------------------------------------------------------------------

class wxFileDialogCustomizeHook
{
public:
    // This method must be overridden to add custom controls to the dialog
    // using the provided customizer object.
    virtual void AddCustomControls(wxFileDialogCustomize& customizer) = 0;

    // This method may be overridden to update the custom controls whenever
    // something changes in the dialog.
    virtual void UpdateCustomControls() { }

    // This method should typically be overridden to save the values of the
    // custom controls when the dialog is accepted.
    virtual void TransferDataFromCustomControls() { }

    virtual ~wxFileDialogCustomizeHook();
};

#endif // _WX_FILEDLGCUSTOMIZE_H_
