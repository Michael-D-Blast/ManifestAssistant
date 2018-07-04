#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    // TODO: Make a nicer UI
    gridLayout = new QGridLayout(this);
    label = new QLabel(this);
    label->setText("Please select the components you'd like to update and specify the branch and tag you'd like to update to.");
    gridLayout->addWidget(label, 0, 0, 1, 5);

    componentName = new QLabel(this);
    componentBranch = new QLabel(this);
    componentTag = new QLabel(this);

    componentName->setText("NAME");
    componentBranch->setText("BRANCH");
    componentTag->setText("TAG");

    gridLayout->addWidget(componentName, 1, 1, 1, 1);
    gridLayout->addWidget(componentBranch, 1, 2, 1, 1);
    gridLayout->addWidget(componentTag, 1, 3, 1, 1);

    for (int i = 0; i < COMPONENTS_MAX_NUM; i++) {
        componentInputters[i].name =  new QComboBox(this);
        componentInputters[i].branch = new QLineEdit(this);
        componentInputters[i].tag = new TagLineEdit(&components, this);

        gridLayout->addWidget(componentInputters[i].name, 3+i, 1, 1, 1);
        gridLayout->addWidget(componentInputters[i].branch, 3+i, 2, 1, 1);
        gridLayout->addWidget(componentInputters[i].tag, 3+i, 3, 1, 1);

        connect(componentInputters[i].name, SIGNAL(currentIndexChanged(int)), componentInputters[i].tag, SLOT(componentChanged(int)));
//        connect(componentInputters[i].name, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxNIndexChanged()));
    }

    ok = new QPushButton(this);
    ok->setText("OK");
    gridLayout->addWidget(ok, 3+COMPONENTS_MAX_NUM, 2, 1, 1, Qt::AlignCenter | Qt::AlignVCenter);
    connect(ok, SIGNAL(clicked()), this, SLOT(oKClicked()));

    setLayout(gridLayout);
}

Dialog::~Dialog()
{
    delete gridLayout;
    delete label;
    delete ok;

    for (int i = 0; i < COMPONENTS_MAX_NUM; i++) {
        delete componentInputters[i].name;
        delete componentInputters[i].branch;
        delete componentInputters[i].tag;
    }
}

// TODO: Change the implementation, create combo boxes dynamically

void Dialog::setComboxItems()
{
    // Get the items

    components = dot.getAllComponentsList();

    for (int i = 0; i < COMPONENTS_MAX_NUM; i++) {

        // Set items for each combo box

        componentInputters[i].name->addItem("NONE");

        for (int c = 0; c < components.size(); c++) {
            componentInputters[i].name->addItem(components[c].getName());
        }
    }
}

void Dialog::oKClicked()
{
    QString name;
    QString branch;
    QString tag;

    for (int i = 0; i < COMPONENTS_MAX_NUM; i++) {
        struct ComponentInputter c = componentInputters[i];

        name = c.name->currentText();
        if (name != "NONE") {
            branch = c.branch->text();
            tag = c.tag->text();
            Component component(name, tag);
            component.setBranchToCommit(branch);

            // Check the validity of branch and tag

            if (!componentInputIsValid(component)) {

                // TODO: Tell user which information is wrong

                QMessageBox::warning(this, "WARN", "Invalid component information", QMessageBox::Yes);

                return;
            }

            dot.setComponentToUpdate(component);
        }
    }

    dot.displayComponentsToUpdate();

    backendThread.setDot(&dot);

    backendThread.start();

    ok->setEnabled(false);
}

bool Dialog::componentInputIsValid(Component component)
{
    QString branchToCommit = component.getBranchToCommit();
    QString tag = component.getTag();

    if (branchToCommit.isEmpty())
        return false;

    if (tag.isEmpty())
        return false;

    // TODO: Check the format of the branch and tag

    return true;
}

