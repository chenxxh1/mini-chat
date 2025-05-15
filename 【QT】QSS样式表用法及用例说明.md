# 【QT】史上最全最详细的QSS样式表用法及用例说明

## 1. Qt样式表语法

**Qt样式表支持各种属性、伪状态和子控件，可以自定义小部件的外观。**

```
        selector { attribute: value }

        selector:选择器，如 QWidget、QPushButton、QGroupBox等

        attribute:属性，如color、background-color、border、padding等

        value:值，与属性对应
1234567
```

## 2. 选择器

### 2.1 可设置样式的部件

| Widget              | 如何设置                                                     |
| ------------------- | ------------------------------------------------------------ |
| QWidget             | 只支持`background`, `background-clip`和`background-origin`属性。 如果你继承于QWidget，那么你需要为你自定义的QWidget提供一个paintEvent，如下所示： ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/2e73a236999fa3610150d6ed662eb1de.png) 如果没有进行QSS样式表设置，上面的代码就是一个空操作。 注意：请确保自定义小部件定义了Q_OBJECT宏。 |
| QAbstractScrollArea | 支持`box model` ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/371f8655004a2525301bc40e52b891e6.png) margin、border-width和padding属性都默认为0。在这种情况下，所有四个矩形(边距、边框、填充和内容)都完全重合。 |
| QCheckBox           | 支持盒子模型。检查指示器可以使用`::indicator`子控件样式化。默认情况下，指示器被放置在左边。`spacing`属性指定检查指示器和文本之间的间距。 |
| QColumnView         | 可以使用`image`属性进行样式化。箭头指示器可以使用 `::left-arrow`子控件和 `::right-arrow` 子控件进行样式化 |
| QComboBox           | combobox周围的框架可以使用box model设置样式。下拉按钮可以使用`::drop-down`子控件设置样式。默认情况下，下拉按钮放置在小部件的内边距矩形的右边。下拉按钮内的箭头标记可以使用`::down-arrow`子控件设置样式。默认情况下，箭头放置在下拉子控件的内容矩形的中心。 |
| QDateEdit           | 看QSpinBox                                                   |
| QDateTimeEdit       | 看QSpinBox                                                   |
| QDialog             | QDialog继承于QWidget，看QWidget                              |
| QDialogButtonBox    | 可以使用`button-layout`属性更改按钮的布局。                  |
| QDockWidget         | 停靠时支持标题栏和标题栏按钮的样式化。可以使用`border`属性对dock小部件边框进行样式化。`::title`子控件可用于自定义标题栏。关闭和浮动按钮分别使用`::close`按钮和`::float`按钮相对于`::title`子控件进行定位。当标题栏垂直时，设置`:vertical`伪类。另外，根据`QDockWidget::DockWidgetFeature`，设置了`:closable`，`:floatable`和`:movable`伪状态。 |
| QDoubleSpinBox      | 看QSpinBox                                                   |
| QFrame              | 支持box model。从4.3开始，在`QLabel`上设置样式表会自动将`QFrame:: framstyle`属性设置为`QFrame::StyledPanel`。 |
| QGroupBox           | 支持box model。标题可以使用`::title`子控件设置样式。默认情况下，标题的放置取决于`QGroupBox::textAlignment`。在可选QGroupBox的情况下，标题包括检查指示器。指示器使用`::indicator`子控件设置样式。`spacing`属性可以用来控制文本和指示符之间的间距。 |
| QHeaderView         | 支持box model。头视图的部分使用`::section`子控件进行样式化。子控件部分支持`:middle`、`:first`、`:last`、`:only-one`、`:next-selected`、`:previous-selected`、`:selected`和`:checked`伪状态。排序指示器可以使用`::up-arrow`和 `::down-arrow` 子控件进行样式化。 |
| QLabel              | 支持box model。不支持`:hover`伪状态。从4.3开始，在QLabel上设置样式表会自动将`QFrame:: framstyle`属性设置为`QFrame::StyledPanel`。 |
| QLineEdit           | 支持box model。选中项的颜色和背景分别使用`selection-color`和`selection-background-color`设置样式。可以使用`lineedit-password-character`属性设置密码字符的样式。可以使用`lineedit-password-mask-delay`更改密码掩码延迟。 |
| QListView           | 支持box model。启用交替行颜色后，可以使用属性`alternative -background-color`设置交替行颜色的样式。选中项的颜色和背景分别使用`selection-color`和`selection-background-color`设置样式。选择行为由`show-decoration-selected`属性控制。使用`::item`子控件对QListView中的项进行更细的控制。 |
| QListWidget         | 看QListView                                                  |
| QMainWindow         | 支持分隔符的样式化。使用QDockWidget时，QMainWindow中的分隔符使用`::separator`子控件进行样式化。 |
| QMenu               | 单个项目使用`::item`子控件设置样式。除了通常支持的伪状态外，item子控件还支持`:selected`、`:default`、`:exclusive`和 `:non-exclusive`伪状态。可选菜单项的指示符使用`::indicator`子控件设置样式。分隔符使用`::separator`子控件设置样式。对于子菜单项，箭头标记使用`::right-arrow` 和`::left-arrow`设置样式。使用`::scroller`对滚动条进行样式化。撕裂式使用`::tearoff`。 |
| QMenuBar            | `spacing`属性指定菜单项之间的间距。使用`::item`子控件对单个项进行样式化。 |
| QMessageBox         | `message - box-text-interaction-flags`属性可用于更改与消息框中的文本的交互。 |
| QProgressBar        | 进度条的块可以使用`::chunk`子控件设置样式。该块显示在小部件的内容矩形上。如果进度条显示文本，请使用`text-align`属性来定位文本。不确定进度条有`:Indeterminate`伪状态设置。 |
| QPushButton         | 支持`:default`、`:flat`、`:checked`伪状态。从5.15开始，`icon`属性可以被设置为覆盖按钮图标。对于带有菜单的QPushButton，菜单指示器使用`::menu-indicator`子控件样式化。可检查按钮的外观可以使用`:open`和`:closed`伪状态定制。 警告：如果你只在一个QPushButton上设置了背景色，背景可能不会出现，除非你设置了border属性为某个值。这是因为，在默认情况下，QPushButton绘制了一个与背景色完全重叠的本机边框。例如： ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/55d701c45580474be7027d488a11b89a.png) |
| QRadioButton        | 检查指示器可以使用`::indicator`子控件样式化。默认情况下，指示器被放置在小部件Contents矩形的左上角。`spacing`属性指定检查指示器和文本之间的间距。 |
| QScrollBar          | 窗口组件的内容矩形被认为是滑块移动的槽。QScrollBar的范围(即宽度或高度，取决于方向)分别使用`width`或`height`属性设置。要确定方向，可以使用`:horizontal`和`:vertical`伪状态。可以使用`::handle`子控件设置滑块的样式。设置`min-width`或`min-height`可以根据方向为滑块提供尺寸约束。 `::add-line`子控件可用于设置按钮的样式以添加行。默认情况下，add-line子控件放置在小部件边框矩形的右上角，取决于`::right-arrow` 或 `::down-arrow`. 。默认情况下，箭头放置在添加行子控件的Contents矩形的中心。 `:: subline`子控件可用于设置按钮的样式以减去一行。默认情况下，subline子控件放置在小部件边框矩形的右下角，取决于`::left-arrow` 或 `::up-arrow`。默认情况下，箭头放置在子行子控件的Contents矩形的中心。`:: subpage`子控件可用于设置滑块减去页面的区域的样式。`::add-page`子控件可用于设置添加页面的滑块区域的样式。 |
| QSizeGrip           | 支持 `width`、`height`和`image` 属性。                       |
| QSlider             | 对于水平滑动，必须提供 `min-width`和 `height`属性。对于垂直滑块，必须提供`min-heigh`t和`width`属性。滑块的凹槽采用`::groove`样式。默认情况下，凹槽位于小部件的内容矩形中。滑块的头部使用`::handle`子控件设置样式。子控件在槽子控件的内容矩形中移动。 |
| QSpinBox            | 向上按钮和箭头可以使用`::up-button`和`::up-arrow`子控件设置样式。默认情况下，向上按钮放置在小部件的内边距矩形的右上角。如果没有明确的尺寸，它将占据其参考矩形高度的一半。向上箭头放置在向上按钮的内容矩形的中心。可以使用`::down-button`和`::down-arrow`子控件为down按钮和箭头设置样式。默认情况下，down按钮放置在小部件的内边距矩形的右下角。如果没有明确的尺寸，它将占据其参考矩形高度的一半。底部箭头放置在底部按钮的内容矩形的中心。 |
| QSplitter           | 分割器的句柄使用`::handle`子控件进行样式化                   |
| QStatusBar          | 只支持`background`属性。可以使用`::item`子控件对单个项的框架进行样式化。 |
| QTabBar             | 单个选项卡可以使用`::tab`子控件设置样式。选项卡支持`:only-one`、`:first`、`:last`、`:middle`、`:previous--selected`、`:next-selected`、`:selected`等伪状态。`:top、:left、:right、:bottom`伪状态取决于选项卡的方向。选择状态的重叠制表符可以使用负边距或绝对位置模式创建。QTabBar的撕裂指示器使用`::tear`子控件进行样式设置。QTabBar为它的滚动条使用了两个QToolButton，可以使用QTabBar QToolButton选择器设置样式。要指定滚动按钮的宽度，可以使用`::scroller`子控件。QTabBar中选项卡的对齐方式使用`alignment`属性设置样式。要更改QTabBar在QTabWidget中的位置，请使用tab-bar子控件(并设置subcontrol-position)。 |
| QTabWidget          | 选项卡部件的框架使用`::pane`子控件设置样式。左角和右角分别使用`::left-corner`和`::right-corner`设置样式。tab bar的位置由`::tab-bar`子控件控制。默认情况下，子控件具有QTabWidget在QWindowsStyle中的位置。要将QTabBar放在中间，请设置tab-bar子控件的subcontrol-position。`:top、:left、:right、:bottom`伪状态取决于选项卡的方向。. |
| QTableView          | 启用交替行颜色后，可以使用属性`alternative-background-color`设置交替行颜色的样式。选中项的颜色和背景分别使用`selection-color`和`selection-background-color`设置样式。QTableView中的拐角部件是作为QAbstractButton实现的，可以使用“`QTableView QTableCornerButton::section`”选择器设置样式。 警告：如果你只在QTableCornerButton上设置了背景色，背景可能不会出现，除非你设置border属性为某个值。这是因为，在默认情况下，QTableCornerButton绘制了一个完全重叠背景色的本机边框。网格的颜色可以使用`gridline-color`属性指定。 |
| QTextEdit           | 所选文本的颜色和背景分别使用`selection-color`和`selection-background-color`进行样式化。. |
| QTimeEdit           | 看QSpinBox                                                   |
| QToolBar            | `:top, :left, :right, :bottom`伪状态取决于工具栏所在的分组区域。`:first，:last，:middle，:only-one`伪状态指示工具栏在一个行组中的位置(参见QStyleOptionToolBar::positionWithinLine)。QToolBar的分隔符使用`::separator`子控件设置样式。手柄(移动工具栏)使用`::handle`子控件设置样式。 |
| QToolButton         | 如果QToolButton有菜单，则可以使用`::menu-indicator`子控件来设置指示器的样式。默认情况下，菜单指示符位于widget的内边距矩形的右下角。如果QToolButton处于QToolButton::MenuButtonPopup模式，则使用`::menu-button`子控件来绘制菜单按钮。`::menu-arrow`子控件用于在菜单按钮内绘制菜单箭头。默认情况下，它位于menu-button子控件内容矩形的中心。当QToolButton显示箭头时，将使用`::up-arrow、::down-arrow、::left-arrow和::right-arrow`子控件。 警告：如果你只在QToolButton上设置了背景色，背景将不会出现，除非你设置border属性为某个值。这是因为，在默认情况下，QToolButton绘制了一个与背景颜色完全重叠的本机边框。例如： ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/52c5933acd246be2f98dc722bdadfdc9.png) |
| QToolBox            | 可以使用`::tab`子控件对单个选项卡进行样式化。选项卡支持`:only-one、:first、:last、:middle、:previous-selected、:next-selected、:selected`伪状态。 |
| QToolTip            | `opacity`属性控制工具提示的不透明度。                        |
| QTreeView           | 当设置交替行颜色 enabled后，可以使用属性`alternative -background-color`设置交替行颜色的样式。选中项的颜色和背景分别使用`selection-color和selection-background-color`设置样式。选择行为由`show-decoration-selected`属性控制。树视图的分支可以使用`::branch`子控件设置样式。::branch子控制支持`:open、:closed、:has-sibling和:has-children`伪状态。使用`::item`子控件对QTreeView中的项进行更细粒度的控制。 |
| QTreeWidget         | 看QTreeView                                                  |

### 2.2 选择器类型

| 选择器     | 示例                        | 说明                                                   |
| ---------- | --------------------------- | ------------------------------------------------------ |
| 通用选择器 | *                           | 匹配所有部件                                           |
| 类型选择器 | QPushButton                 | 匹配所有QPushButton示例及它的所有子类                  |
| 属性选择器 | QPushButton[flat = “false”] | 匹配QPushButton的属性flat为false的示例                 |
| 类选择器   | .QPushButton                | 匹配所有QPushButton示例，但不包括它的子类              |
| ID选择器   | QPushButton#myButton        | 匹配所有QPushButton中myButton对象的实例                |
| 后代选择器 | QDialog QPushButton         | 匹配所有QPushButton实例，它们必须是QDialog的子孙部件   |
| 孩子选择器 | QDialog>QPushButton         | 匹配所有QPushButton实例，它们必须是QDialog直系孩子部件 |

### 2.3 子控件

```
子控件是双冒号"::"`
对于一些复杂的部件修改样式，可能需要访问它们的子部件，如[QComboBox](https://so.csdn.net/so/search?q=QComboBox&spm=1001.2101.3001.7020)的下拉按钮，QSpinBox的向上、向下箭头灯。
如：下拉按钮设置图片属性：`QComboBox::drop-down{image:url(dropdown.png)}
```

| 子控件           | 描述                                                         |
| ---------------- | ------------------------------------------------------------ |
| ::add-line       | 在QScrollBar中跳转下一行的按钮                               |
| ::add-page       | 在QScrollBar中滑动条和add-line之间的区域                     |
| ::branch         | 在QTreeView中的分支指示器                                    |
| ::chunk          | 在QProgressBar中的进度块                                     |
| ::close-button   | 在QDockWidget或QTabBar选项卡的关闭按钮                       |
| ::corner         | 在QAbstractScrollArea中两个滚动条之间的角落                  |
| ::down-arrow     | 在QComboBox、QHeaderView(排序指示器)、QScrollBar或QSpinBox的向下箭头 |
| ::down-button    | 在QScrollBar或QSpinBox中的向下按钮                           |
| ::drop-down      | 在QComboBox中的下拉框                                        |
| ::float-button   | 在QDockWidget中的浮动按钮                                    |
| ::groove         | 在QSlider中的滑动槽                                          |
| ::indicator      | 在QAbstractItemVIew、QCheckBox、QRadioButton、可选中的菜单项或可选中的QGroupBox中的指示器 |
| ::handle         | 在QScrollBar、QSplitter和QSlider中的操作条(滑动条)           |
| ::icon           | 在QAbstractItemVIew或QMenu中的图标                           |
| ::item           | 在QAbstractItemVIew、QMenuBar、QMenu或QStatuBar中的一项      |
| ::left-arrow     | 在QScrollBar中的向左箭头                                     |
| ::left-corner    | 在QTabWidget中的左上角                                       |
| ::menu-arrow     | 带有菜单的QToolButton中的箭头                                |
| ::menu-button    | 在QToolButton中的菜单按钮                                    |
| ::menu-indicator | 在QPushButton中的菜单指示器                                  |
| ::right-arrow    | 在QMenu或QScrollBar中的向右箭头                              |
| ::pane           | 在QTabWidget中的边或框                                       |
| ::right-corner   | 在QTabWidget中的右上角                                       |
| ::scroller       | 在QMenu或QTabBar中的滚动条                                   |
| ::section        | 在QHeaderView中的区块                                        |
| ::separator      | 在QMenu或QMainWIndow中分隔条                                 |
| ::sub-line       | 在QScrollBar中跳转上一行的按钮                               |
| ::sub-page       | 在QScrollBar中滑动条和sub-line之间的区域                     |
| ::tab            | 在QTabBar或QToolBox中选项卡                                  |
| ::tab-bar        | 在QTabWidget中的选项卡栏                                     |
| ::tear           | 在QTabBar中的tear指示器                                      |
| ::tearoff        | 在QMenu中的tear-off指示器                                    |
| ::text           | 在QAbstractItemView中的文本                                  |
| ::title          | 在QGroupBox或QDockWidget中的标题栏                           |
| ::up-arrow       | 在QComboBox、QHeaderView(排序指示器)、QScrollBar或QSpinBox的向上箭头 |
| ::up-button      | 在QScrollBar或QSpinBox中的向上按钮                           |

### 2.4 伪状态

```
伪状态使用单冒号":"
```

1. 选择器可以使用状态来限制在部件的指定状态上的应用。伪状态在选择器之后，用冒号隔离。如：
   鼠标悬停在按钮上时其按钮的颜色为白色：`QPushButton:hover{color:white}`
   鼠标不悬停在按钮上时其按钮的颜色为l蓝色(!表否定)：`QPushButton:!hover{color:blue}`
2. 伪状态可多个连用，达到逻辑与效果。如：
   鼠标悬停在一个被选中的QCheckBox部件上时才应用规则：`QCheckBox:hover:checked{color:white}`
3. 伪状态可通过逗号达到逻辑或效果。如：`QCheckBox:hover,checked{color:white}`
4. 伪状态和子部件联合使用。如：`QComboBox::drop-down:hover{image:url(dropdown_bright.png)}`

| 伪状态             | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| :active            | 此状态在小部件驻留在活动窗口时设置                           |
| :adjoins-item      | 此状态在QTreeView的::branch与一个item相邻时设置              |
| :alternate         | 当QAbstractItemView::alternatingRowColors()设置为真时，在绘制QAbstractItemView的行时，为每个交替行设置此状态 |
| :bottom            | 此item位于底部。例如，QTabBar有位于底部的选项卡              |
| :checked           | 此item被选中。例如，QAbstractButton的checked状态             |
| :closable          | 此item可以被关闭。例如，QDockWidget的QDockWidget::DockWidgetClosable特性开启时 |
| :closed            | 此item处于关闭状态。例如，QTreeView中未展开的item            |
| :default           | 此item的默认状态。例如，一个default的QPushButton或QMenu中的一个默认动作 |
| :disabled          | 此item被禁用时的状态                                         |
| :editable          | 如QComboBox是可编辑的                                        |
| :edit-focus        | 此item具有编辑焦点(参考QStyle::State_HasEditFocus)。此状态仅对Qt扩展应用程序可用 |
| :enabled           | 此item已启用                                                 |
| :exclusive         | 此item是一个独占项组的一部分。例如，独占QActionGroup中的菜单项 |
| :first             | 此item是列表中的第一项。例如，QTabBar中的第一个选项卡        |
| :flat              | 此item是平的。例如，一个扁平的QPushButton                    |
| :floatable         | 此item可以浮动。例如，QDockWidget的QDockWidget::DockWidgetFloatable的特性开启时 |
| :focus             | 此item具有输入焦点                                           |
| :has-children      | 此item具有子对象。例如，QTreeView中具有子项的项              |
| :has-sibling       | 此item具有兄弟对象。例如，QTreeView中与之相邻的项            |
| :horizontal        | 此item处于水平方向                                           |
| :hover             | 鼠标悬浮在此item上                                           |
| :indeterminate     | 此item处于不确定状态。例如，QCheckBox或QRadioButton被部分选中 |
| :last              | 此item是列表中的最后一项。例如，QTabBar中的最后一个选项卡    |
| :left              | 此item位于左侧。例如，QTabBar有位于左侧的选项卡              |
| :maximized         | 此item处于最大化状态。例如，一个最大化的QMdiSubWindow        |
| :middle            | 此item是列表中的中间一项。例如，一个不在QTabBar中的开头或结尾的选项卡 |
| :minimized         | 此item处于最小化状态。例如，一个最小化的QMdiSubWindow        |
| :movable           | 此item可以被移动。例如， QDockWidget的QDockWidget::DockWidgetMovable特性开启时 |
| :no-frame          | 此item没有边框。例如，没有边框的QSpinBox或QLineEdit          |
| :non-exclusive     | 此item是一个非独占项组的一部分。例如，非独占QActionGroup中的菜单项 |
| :off               | 对可以切换的items，这适用于处于off状态的item                 |
| :on                | 对可以切换的items，这适用于处于on状态的widget                |
| :only-one          | 此item是列表中的唯一的一项。例如，一个在QTabBar中单独的选项卡 |
| :open              | 此item处于打开状态。例如，QTreeView中的展开项，或带有菜单的QComboBox或QPushButton |
| :next-selected     | 此item是列表中的下一个被选中的项。例如，在QTabBar中当前选项卡的下一个要选中的选项卡 |
| :pressed           | 鼠标正在按压在此item上                                       |
| :previous-selected | 此item是列表中的上一个被选中的项。例如，在QTabBar中当前选项卡的上一个要选中的选项卡 |
| :read-only         | 此item处于只读或不可编辑状态。例如，一个只读QLineEdit或不可编辑的QComboBox |
| :right             | 此item位于右侧。例如，QTabBar有位于右侧的选项卡              |
| :selected          | 此item处于选中状态。例如，一个在QTabBar中被选中的选项卡或一个在菜单中被选中的菜单项 |
| :top               | 此item位于顶部。例如，QTabBar有位于顶部的选项卡              |
| :unchecked         | 此item处于未被选中状态                                       |
| :vertical          | 此item处于垂直方向                                           |
| :window            | 小部件是一个窗口(例如，一个顶层小部件)                       |

## 3. 属性列表

[QSS官方参考](https://doc.qt.io/qt-6/stylesheet-reference.html)
对于不理解的地方可以参考CSS：[CSS参考手册（里面有测试，可以很直观的看到不同属性值的区别）](https://www.w3school.com.cn/cssref/pr_background-clip.asp)

| attribute属性                               | 类型                                 | 描述                                                         |
| ------------------------------------------- | ------------------------------------ | ------------------------------------------------------------ |
| alternate-background-color                  | Brush                                | 交替背景色。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/40e9ad0927170ea40107dbdb1a559f09.png) |
| background                                  | Background                           | 背景，相当于指定background-color、background-image、background-repeat和background-position。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/73c4ca3d7e09c9a1ec00b31ba4b4c9e2.png) |
| background-color                            | Brush                                | 背景颜色，例如： ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/61090184c34eb92e70ee6ebed7c32bbd.png) |
| background-image                            | Url                                  | 背景图像（图像的半透明部分会被背景颜色穿透）。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/903854c17365268ea1637ff786e0d301.png) |
| background-repeat                           | Repeat                               | 背景图像如何充满背景矩形，如果未指定此属性，背景图像将在两个方向上重复(repeat)。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/f1375a65959ebeb45a79805868d8b80f.png) |
| background-position                         | Alignment                            | 背景图像在背景原点矩形内的对齐。如果未指定此属性，则对齐方式为左上角。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/6bb3bc799e9d53c657ce2864e20adf32.png) |
| background-attachment                       | Attachment                           | 背景图像是否相对于视口进行滚动或固定。默认情况下，背景图像随视口滚动。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/bddde5bd90ed5229dc0dd0c5b88c643c.png) |
| background-clip                             | Origin                               | 规定背景的绘制区域。如果未指定此属性，则默认为border。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/74f2ca6d220cfd79428a1fe0f81506ac.png) border：背景被裁剪到边框盒 padding：背景被裁剪到内边距框 content：背景被裁剪到内容框 |
| background-origin                           | Origin                               | 背景图像相对于什么来定位。如果未指定此属性，则默认为 padding 填充。 padding：背景图像相对于内边距框来定位 border：背景图像相对于边框盒来定位 content：背景图像相对于内容框来定位 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/2b282228878b1ec915c6e3a7d6827036.png) |
| border                                      | Border                               | 设置 4 个边框的样式，按照一下顺序进行设置：border-width，border-style，border-color。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/2561d1e7893a18ecb6b20cc7b727c57d.png) |
| border-top                                  | Border                               | 顶部边框                                                     |
| border-bottom                               | Border                               | 底部边框                                                     |
| border-right                                | Border                               | 右边边框                                                     |
| border-left                                 | Border                               | 左边边框                                                     |
| border-color                                | Box Colors                           | 边框的颜色。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/7990a70c224cf9dfed8f03bdea3d04de.png) |
| border-top-color                            | Brush                                | 边框上边缘颜色                                               |
| border-bottom-color                         | Brush                                | 边框下边缘颜色                                               |
| border-right-color                          | Brush                                | 边框右边缘颜色                                               |
| border-left-color                           | Brush                                | 边框左边缘颜色                                               |
| border-image                                | Border Image                         | 填充边框的图像                                               |
| border-radius                               | Radius                               | 边角的半径，即边角圆弧。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/d4b4a6b3ca1fa228b3f5e42b840ca1e6.png) |
| border-top-left-radius                      | Radius                               | 左上角圆弧                                                   |
| border-top-right-radius                     | Radius                               | 右上角圆弧                                                   |
| border-bottom-right-radius                  | Radius                               | 右下角圆弧                                                   |
| border-bottom-left-radius                   | Radius                               | 左下角圆弧                                                   |
| border-style                                | Border Style                         | 边框边缘的样式，若未指定默认为none。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9499e233671d4c7fe372b651dff2f422.png) |
| border-top-style                            | Border Style                         | 边框上边缘的样式                                             |
| border-bottom-style                         | Border Style                         | 边框下边缘的样式                                             |
| border-right-style                          | BorderStyle                          | 边框右边缘的样式                                             |
| border-left-style                           | BorderStyle                          | 边框左边缘的样式                                             |
| border-width                                | Box Lengths                          | 边框的宽度。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/1c57aab2b4219936fa026447d12d77db.png) |
| border-top-width                            | Box Lengths                          | 上边框的宽度                                                 |
| border-bottom-width                         | Box Lengths                          | 下边框的宽度                                                 |
| border-right-width                          | Box Lengths                          | 右边框的宽度                                                 |
| border-left-width                           | Box Lengths                          | 左边框的宽度                                                 |
| bottom                                      | Length                               | 在部件边底部其子控件向上的偏移量，即元素与底部边缘的距离。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/4e27d3b54624fefe62db741763ae6066.png) |
| top                                         | Length                               | 在部件边顶部其子控件向下的偏移量，即元素与顶部边缘的距离。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/5a4c65312ab7aa3a0c470839dda9a623.png) |
| right                                       | Length                               | 在部件右边其子控件向左的偏移量，即元素与右边缘的距离。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/c0b9df530888086e83531972ba7792f9.png) |
| left                                        | Length                               | 在部件左边其子控件向右的偏移量，即元素与左边缘的距离。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/643a3697b69e99c2cfb4e999ae91e0e8.png) |
| button-layout                               | Number                               | QDialogButtonBox或QMessageBox中的按钮布局。取值为0 (WinLayout)、1 (MacLayout)、2 (KdeLayout)、3 (GnomeLayout)和5 (AndroidLayout)。如果未指定此属性，它默认为SH_DialogButtonLayout样式提示的当前样式所指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/c51fa54c21f44ab539e848ca0316ad21.png) |
| color                                       | Brush                                | 渲染文本的颜色，默认值为黑色。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/46a5cf2345f01d4dda37517655200846.png) |
| dialogbuttonbox-buttons-have-icons          | Boolean                              | QDialogButtonBox中的按钮是否显示图标如果这个属性被设置为1,QDialogButtonBox的按钮显示图标;如果设置为0，则不显示图标。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/8beec7b7ab4edb6822804ea16c130cc3.png) |
| font                                        | Font                                 | 设置字体。相当于指定font-family、font-size、font-style和/或font-weight。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/e26e20e15138f3c74a5ccafa79d5ba56.png) |
| font-family                                 | String                               | 字体类型。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/8dd50cab483fff2d38bcd0d7612daf29.png) |
| font-size                                   | Font Size                            | 字体大小。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9d3f411998828e4f8192996acc0f53b5.png) |
| font-style                                  | Font Style                           | 字体风格(倾斜）。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/1fb7a817211175b8b3b79e62dd5a80d4.png) |
| font-weight                                 | Font Weight                          | 字体的粗细。                                                 |
| gridline-color*                             | Color                                | QTableView中网格线的颜色。如果没有指定此属性，它默认为SH_Table_GridLineColor样式提示的当前样式所指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9bcd0a19942b7e69eefcd57ce8c212f4.png) |
| height                                      | Length                               | 子部件的高度。如果希望小部件具有固定的高度，请将min-height和max-height设置为相同的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/7a5dffc9db80a3caba81c1d7d8a5cd7e.png) |
| width                                       | Length                               | 子控件(在某些情况下是小部件)的宽度。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/60a2bec3157503b4768dad388b85799e.png) |
| icon                                        | Url+                                 | 所使用的图标，用于具有图标的小部件。目前唯一支持此属性的小部件是QPushButton。 |
| icon-size                                   | Length                               | 小部件中图标的宽度和高度。可以使用此属性设置以下小部件的图标大小：QCheckBox、QListView、QPushButton、QRadioButton、QTabBar、QToolBar、QToolBox、QTreeView。 |
| image*                                      | Url+                                 | 在子控件的内容矩形中绘制图像。image属性接受一个url列表或者一个svg。绘制的实际图像使用与QIcon相同的算法。图像从不放大，但在必要时总是缩小。如果指定了svg，则图像会被缩放为内容矩形的大小。警告：渲染SVG图像需要QIcon SVG插件。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/91cd03ad30d2237354766451fd8bc208.png) 隐式设置down-button的大小为spindown.png的大小 |
| image-position                              | Alignment                            | 图像位置的对齐方式                                           |
| lineedit-password-character*                | Number                               | QLineEdit密码字符作为Unicode数字。如果没有指定此属性，它默认为SH_LineEdit_PasswordCharacter样式提示的当前样式所指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/472a9aaaeff6aa8a48b8681088d3dc9a.png) |
| lineedit-password-mask-delay*               | Number                               | 在lineedit-password-character应用到可见字符之前，QLineEdit密码掩码延迟毫秒。如果未指定此属性，它默认为SH_LineEdit_PasswordMaskDelay样式提示的当前样式所指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/875344532b02a6e77d5ee44d8462d692.png) |
| margin                                      | Box Lengths                          | 设置元素的4个外边距。相当于指定margin-top、margin-right、margin-bottom和margin-left。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/7579a42841b523477e2348088a20edbc.png) |
| margin-top                                  | Length                               | 上边距                                                       |
| margin-right                                | Length                               | 右边距                                                       |
| margin-bottom                               | Length                               | 下边距                                                       |
| margin-left                                 | Length                               | 左边距                                                       |
| max-height                                  | Length                               | 小部件或子控件（例如Item）的最大高度。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/191a485374898310f7d27d0c8eb0522e.png) |
| max-width                                   | Length                               | 小部件或子控件（例如Item）的最大宽度。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/b0330482c93fc2124fb567d6be140415.png) |
| min-height                                  | Length                               | 小部件或子控件（例如Item）的最小高度。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/d360878d08b43a7e58107e6a3e607ed8.png) |
| min-width                                   | Length                               | 小部件或子控件（例如Item）的最小宽度。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/982cd33174d0b7f294099271be5fc8a9.png) |
| messagebox-text-interaction-flags*          | Number                               | 消息框中文本的交互行为。可能的值基于Qt::TextInteractionFlags。如果未指定此属性，则默认为SH_MessageBox_TextInteractionFlags样式提示的当前样式所指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/70fa5f152a5a030eaae273460e37c073.png) |
| opacity*                                    | Number                               | 小部件的不透明度。取值范围为0(透明)~ 255(不透明)。目前，这只支持`tooltips`。如果未指定此属性，它默认为SH_ToolTipLabel_Opacity样式提示的当前样式指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/e438bb726bf78157cea16c0ed30cb03d.png) |
| outline                                     |                                      | 边框的轮廓                                                   |
| outline-color                               | Color                                | 轮廓的颜色                                                   |
| outline-offset                              | Length                               | 轮廓与小部件边界的偏移量                                     |
| outline-style                               |                                      | 指定用于绘制轮廓的模式                                       |
| outline-radius                              |                                      | 在轮廓中添加圆角                                             |
| outline-bottom-left-radius                  | Radius                               | 轮廓左下角的圆角                                             |
| outline-bottom-right-radius                 | Radius                               | 轮廓右下角的圆角                                             |
| outline-top-right-radius                    | Radius                               | 轮廓右上角的圆角                                             |
| outline-top-left-radius                     | Radius                               | 轮廓左上角的圆角                                             |
| padding                                     | Box Lengths                          | 小部件的填充。相当于指定填充顶部、填充右侧、填充底部和填充左侧。如果未指定此属性，则默认为0。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/7081afd87e64fdf728bf48d95a9c091f.png) |
| padding-top                                 | Length                               | 顶部填充                                                     |
| padding-right                               | Length                               | 右侧填充                                                     |
| padding-bottom                              | Length                               | 底部填充                                                     |
| padding-left                                | Length                               | 左侧填充                                                     |
| paint-alternating-row-colors-for-empty-area | bool                                 | QTreeView是否为空区域(即没有项目的区域)绘制交替的行色        |
| posotion                                    | relative \| absolute                 | 使用左、右、上和下指定的偏移量是相对坐标还是绝对坐标。如果未指定此属性，则默认为relative。 |
| selection-background-color*                 | Brush                                | 所选文本或项的背景。如果未设置此属性，则默认值是为调色板palette的Highlight角色设置的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/0dbecaeb262d2148887fedbba3054db5.png) |
| selection-color*                            | Brush                                | 所选文本或项的前景。如果未设置此属性，则默认值是为调色板palette的HighlightedText角色设置的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/e1c9b61d1c28a26de14a5318d832046b.png) |
| show-decoration-selected*                   | Boolean                              | 控制QListView中的选择是覆盖整行还是仅覆盖文本的范围。如果未指定此属性，它默认为SH_ItemView_ShowDecorationSelected样式提示的当前样式指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/d41ee9ac65c204c9aa434a0364df5d8b.png) |
| spacing*                                    | Length                               | 小部件中的内部间距。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9c2889279b3d0a0748b37d169658bec0.png) |
| subcontrol-origin*                          | Origin                               | 父元素中子控件的起始矩形。如果未指定此属性，则默认为填充。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/f918d986256265f4fa6bfd1f7b545177.png) |
| subcontrol-position*                        | Alignment                            | 子控件在subcontrol-origin指定的原点矩形内的对齐。如果未指定此属性，则默认为依赖于子控件的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/1833bd6080eec71729d91d056489aafb.png) |
| titlebar-show-tooltips-on-buttons           | bool                                 | 工具提示是否显示在窗口标题栏按钮上。                         |
| widget-animation-duration*                  | Number                               | 动画应该持续多长时间(以毫秒为单位)。值等于零意味着动画将被禁用。如果没有指定此属性，它默认为SH_Widget_Animation_Duration样式提示的当前样式指定的值。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/12ac49b013e76cfdb17302fd4b682aac.png) |
| text-align                                  | Alignment                            | 小部件内容中的文本和图标的对齐方式。 ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/f5b63c58ffe512a3583457e48c28ad9c.png) |
| text-decoration                             | none underline overline line-through | 附加文本效果                                                 |
| -qt-background-role                         | PaletteRole                          | 基于所选角色的子控件或小部件的背景色。                       |
| -qt-style-features                          | list                                 | 要在其上应用特定于qt的样式的CSS属性列表。注意:列表只能包含非基于像素图的属性。 |

## 4. 冲突解决

几个样式规则对相同的属性指定不同的值时会产生冲突。如

```cpp
QPushButton#okButton{color:gray}
QPushButton{color:red}
12
```

冲突原则：特殊的选择器优先。此例中QPushButton#okButton代表的是单一对象，而不是一个类的所有实例，所以okButton的文本颜色会是灰色的。同样的有伪状态的比没有伪状态的优先。

## 5. QSS示例

[点这里：QSS用例显示](https://doc.qt.io/qt-5/stylesheet-examples.html#customizing-qabstractscrollarea)

## 6. 如何为全局添加QSS样式（全局加载QSS的方法）

在Resource文件中添加qss文件，构造函数中直接调用

```cpp
void MainWindow::loadStyleSheet(const QString &styleSheetFile){  //styleSheetFile是qss文件的Path
	QFile file(styleSheetFile);
	if(file.open(QFile::ReadOnly)){
		//如果在MainWindow使用了setStyleSheet，对qss文件的内容进行追加
		//QString styleSheet = this->styleSheet();
		//styleSheet += QLatin1String(file.readAll());//读取样式表文件
		QString styleSheet = QLatin1String(file.readAll());  //QByteArray可直接转换为QString
    	this->setStyleSheet(styleSheet);
    	file.close();
	}else{
		QMessageBox::information(this,"tip","Failed to open file");
	}
}
12345678910111213
```

注意：

1. 对同一个对象调用多次setStyleSheet，先前的设置将无效，只有最后设置的才会生效。

```cpp
this->setStyleSheet(styleSheet1);
this->setStyleSheet(styleSheet2);  //styleSheet1中的所有设置完全失效，使用styleSheet2的定义
12
```

1. 若是不同的对象，可以在函数中多次调用setStyleSheet是可以的，是对不同的对象进行样式设置，但是对同一对象多次调用setStyleSheet，最后设置的才有效。例如：

```cpp
//对不同的对象进行样式设置
QDialog dia1;
QDialog dia2;
dia1.setStyleSheet(styleSheet1);
dia2.setStyleSheet(styleSheet2);
12345
//对同一对象dia1调用setStyleSheet，只有最后调用的setStyleSheet才有效，即styleSheet2会覆盖掉styleSheet1，使得styleSheet2设置成功
QDialog dia1;
dia1.setStyleSheet(styleSheet1);
dia1.setStyleSheet(styleSheet2);
1234
```

