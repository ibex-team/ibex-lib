///
/// \file vibes.h
/// \brief Vibes C++ API Header
/// \author Vincent Drevelle, Jeremy Nicola
/// \date 2013-2014
/// \version 0.2.0beta
///

#ifndef VIBES_CPP_API_H
#define VIBES_CPP_API_H

/*! \mainpage
 *
 * This is VIBes C++ API documentation. It is generated from source code.
 * The "Modules" section of this documentation lists the VIBes commands thematically.
 *
 */

///
/// Vibes API configuration defines (def or undef as needed)
///

/// Generate "vibesFunctionName(arg1,arg2,argN,...)" macros for each Vibes function,
/// to allow Matlab style variadic calls with properies.
/// Also defines the "vibesParams(...)" macro to help specifying parameters.
/// Undef this if you don't want to define macros
#define VIBES_GENERATE_vibesXXX_MACROS

/// Verbose debugging functions (logs to standard output), only useful for library devs
#undef VIBES_DEBUG_API


///
/// Includes
///

#include <vector>
#include <string>
#include <map>
#include <sstream>

#ifdef VIBES_DEBUG_API
#include <iostream>
#define VIBES_DEBUG(i) std::cout << "VibesDebug " << __FILE__ << "(" << __LINE__ << ") : " << i << std::endl;
#else
#define VIBES_DEBUG(i)
#endif

/** \ingroup connection params figure drawing */
namespace vibes {

  /** @defgroup params VIBes properties key-value classes
   *
   *  @{
   */

    class Params; // Forward declaration of Params
    /*!
     * A class to hold any type supported by vibes properties system, an to provide JSON serialization
     */
    class Value {
        enum value_type_enum{
            vt_none, vt_integer, vt_string, vt_decimal, vt_array, vt_object
        } _type;

        union {
        int _integer;
        double _decimal;
        const Params *_object;
        };
        std::string _string;
        std::vector<Value> _array;

    public:
        Value() : _type(vt_none) {}
        Value(int i) : _integer(i), _type(vt_integer) {}
        Value(const double &d) : _decimal(d), _type(vt_decimal) {}
        Value(const std::string &s) : _string(s), _type(vt_string) {}
        Value(const char *s) : _string(s), _type(vt_string) {}
        Value(const std::vector<Value> &a) : _array(a), _type(vt_array) {}
        template <typename T> Value(const std::vector<T> &v) : _array(v.begin(),v.end()), _type(vt_array) {}
        /*explicit */Value(const Params &p) : _object(&p), _type(vt_object) {}
        bool empty() {return (_type == vt_none);}
        std::string toJSONString() const;
    };

    /*!
     * A static array that can be cast into \class Value
     *
     * This enables easy intialisation of array of parameters with the brace
     * syntax.
     * Example:
     *\code{.cpp}
     * (Vec<int,4>){1,2,3,4}
     *\endcode
     */
    template<typename T, int N>
    struct Vec {
        T _data[N];
        operator Value() {return Value(std::vector<Value>(_data,_data+N));}
    };


    /*!
     * \name Useful types for colors, vectors and points
     */
    ///@{
    typedef Vec<int,3> RGB;         ///< A RGB triplet [0..255]
    typedef Vec<int,4> RGBA;        ///< A RGBA triplet [0..255]
    typedef Vec<double,2> Vec2d;    ///< A 2D floating point vector
    typedef Vec<double,3> Vec3d;    ///< A 3D floating point vector
    typedef Vec<double,4> Vec4d;    ///< A 4D floating point vector

    ///@}

    /*!
     * A class that holds a list of parameters, and provides JSON serialization
     */
    class Params {
        class NameHelper;
        typedef std::map<std::string, Value> KeyValueMap;
        KeyValueMap _values;
    public:
        Params() {}
        template<typename T> Params(const std::string & name, const T &p) {_values[name] = p;}
        Value & operator[](const std::string &key) {return _values[key];}
        Value pop(const std::string &key, const Value &value_not_found = Value());
        NameHelper operator, (const std::string &s);
        Params& operator& (const Params &p) { for(KeyValueMap::const_iterator it = p._values.begin(); it != p._values.end(); ++it) _values[it->first] = it->second; return *this;}
        std::size_t size() const { return _values.size(); }
        std::string toJSON() const;
    };

    /*!
     * A helper class for comma-separated key-value input of parameters
     */
    class Params::NameHelper {
    public:
        Params &_params;
        std::string _name;
        NameHelper(Params & list, const std::string & name) : _params(list), _name(name) {}
        Params & operator, (const Value &value) { _params[_name] = value; return _params; }
        #ifdef VIBES_GENERATE_vibesXXX_MACROS
        // Conversion of a singleton parameter to a color (for use with macros)
        operator Params&() {_params["format"] = _name; return _params;}
        #endif
    };

    inline Params::NameHelper Params::operator, (const std::string &s) { return NameHelper(*this, s); }


  /** @} */ // end of group params


  // Convenience drawing functions with color selection
#define VIBES_COLOR_PARAM_NAME "format"
#define VIBES_FUNC_COLOR_PARAM_1(func_name, T1, a) \
  void func_name(T1 a, Params params); \
  inline void func_name(T1 a, \
              const std::string &format=std::string(), Params params=Params()) {func_name(a,(params,VIBES_COLOR_PARAM_NAME,format));}
#define VIBES_FUNC_COLOR_PARAM_2(func_name, T1, a, T2, b) \
  void func_name(T1 a, T2 b, Params params); \
  inline void func_name(T1 a, T2 b, \
              const std::string &format=std::string(), Params params=Params()) {func_name(a,b,(params,VIBES_COLOR_PARAM_NAME,format));}
#define VIBES_FUNC_COLOR_PARAM_3(func_name, T1, a, T2, b, T3, c) \
  void func_name(T1 a, T2 b, T3 c, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, \
              const std::string &format=std::string(), Params params=Params()) {func_name(a,b,c,(params,VIBES_COLOR_PARAM_NAME,format));}
#define VIBES_FUNC_COLOR_PARAM_4(func_name, T1, a, T2, b, T3, c, T4, d) \
  void func_name(T1 a, T2 b, T3 c, T4 d, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, T4 d, \
              const std::string &format=std::string(), Params params=Params()) {func_name(a,b,c,d,(params,VIBES_COLOR_PARAM_NAME,format));}
#define VIBES_FUNC_COLOR_PARAM_5(func_name, T1, a, T2, b, T3, c, T4, d, T5, e) \
  void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, \
              const std::string &format=std::string(), Params params=Params()) {func_name(a,b,c,d,e,(params,VIBES_COLOR_PARAM_NAME,format));}
#define VIBES_FUNC_COLOR_PARAM_6(func_name, T1, a, T2, b, T3, c, T4, d, T5, e, T6, f) \
  void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, Params params); \
  inline void func_name(T1 a, T2 b, T3 c, T4 d, T5 e, T6 f, \
              const std::string &format=std::string(), Params params=Params()) {func_name(a,b,c,d,e,f,(params,VIBES_COLOR_PARAM_NAME,format));}


  /** @defgroup connection Starting and ending VIBes
   *
   *  @brief Functions for establishing connection with the VIBes viewer application.
   *
   *  VIBes drawing is performed by the VIBes viewer application. The viewer has to be
   *  running for VIBes graphics to be displayed.
   *  - Before using any VIBes commands in your application, the VIBes \c beginDrawing()
   *    command has to be executed once (e.g. during the application initialization).
   *  - When your application is done with drawing, the \c endDrawing() command has to
   *    be called (e.g. when the application quits).
   *  @{
   */

  /// Start VIBes in connected mode: connects to the VIBes viewer.
  void beginDrawing();
  /// Start VIBes in file saving mode. All commands are saved to the specified file.
  void beginDrawing(const std::string &fileName);
  
  /// Close connection to the viewer or the drawing file.
  void endDrawing();
  
  /** @} */ // end of group connection


  /** @defgroup figure Figure and view management
   *
   *  @brief VIBes drawings are performed on figures. These functions provide a way to create,
   *         save and manipulate figures.
   *  @{
   */

  ///
  /// @name Figure management
  /// @{

  /// Create a new figure named \a figureName
  void newFigure(const std::string &figureName = std::string());

  /// Clears the contents of the figure \a figureName, or the current figure if argument is given.
  void clearFigure(const std::string &figureName = std::string());

  /// Close the figure named \a figureName, or the current figure if no argument is given.
  void closeFigure(const std::string &figureName = std::string());

  /// Export to \a fileName the contents of the figure \a figureName. If \a figureName is omitted,
  /// operation applies to current figure. If \a fileName is omitted, a "Save As" window will be displayed.
  void saveImage(const std::string &fileName = std::string(), const std::string &figureName = std::string());

  /// Select \a figureName as the current figure. Drawing operations will then apply to \a figureName.
  void selectFigure(const std::string &figureName);

  /// @}
  /// @name View settings
  /// @{

  /// Set axes limits to the bounding box of the drawing
  void axisAuto(const std::string &figureName = std::string());

  /// Specify the rectangle to be displayed: Lower-left corner (\a x_lb, \a y_lb) and a upper-right corner (\a x_ub, \a y_ub).
  void axisLimits(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &figureName = std::string());

  /// Set x-axis and y-axis labels to respectively \a x_label and \a y_label
  void axisLabels(const std::string &x_label, const std::string &y_label, const std::string &figureName = std::string());
  /// Set axis labels according to the list provided in \a labels
  void axisLabels(const std::vector<std::string> &labels, const std::string &figureName = std::string());

  /// @}
  ///
  ///

  /** @} */ // end of group figure



  /** @defgroup drawing Drawing and editing functions
   *
   *  @brief Functions to draw primitives on Vibes figures, group objects, modify graphics
   *         attributes and remove objects from a figure.
   *  @{
   */

  ///
  /// @name Primitive drawing functions
  /// @brief Functions for drawing boxes, ellipses, lines. By default, all these functions
  ///        operate on the current figure. The optional agument \a format provides quick
  ///        graphics attribute setup. Additional parameters can be provided in the optional
  ///        last argument \a params.
  /// @{

  /// Draw a 2-D box with a left lower corner at (\a x_lb, \a y_lb) and a right upper corner at (\a x_ub, \a y_ub)
  VIBES_FUNC_COLOR_PARAM_4(drawBox,const double &,x_lb, const double &,x_ub, const double &,y_lb, const double &,y_ub)
  /// Draw a N-D box from a list of \a bounds in the form (x_lb, x_ub, y_lb, y_ub, z_lb, z_ub, ...)
  VIBES_FUNC_COLOR_PARAM_1(drawBox,const std::vector<double> &,bounds)

  /// Draw an ellipse centered at (\a cx, \a cy), with semi-major and minor axes \a a and \a b, and rotated by \a rot degrees
  VIBES_FUNC_COLOR_PARAM_5(drawEllipse,const double &,cx, const double &,cy, const double &,a, const double &,b, const double &,rot)
  /// Draw a 2-D confidence ellipse centered at (\a cx, \a cy), with covariance \a sxx, \a sxy, \a syy and scale \a K
  VIBES_FUNC_COLOR_PARAM_6(drawConfidenceEllipse,const double &,cx, const double &,cy,
                                                 const double &,sxx, const double &,sxy, const double &,syy,
                                                 const double &,K/*=3.0*/)
  /// Draw a N-D confidence ellipse centered at \a center, with covariance in \a cov and scale \a K
  VIBES_FUNC_COLOR_PARAM_3(drawConfidenceEllipse,const std::vector<double> &,center,
                                                 const std::vector<double> &,cov,
                                                 const double &,K/*=3.0*/)
  /// Draw a circle centered at (\a cx, \a cy), with radius \a r
  VIBES_FUNC_COLOR_PARAM_3(drawCircle,const double &,cx, const double &,cy, const double &,r)

  /// Draw a list of N-D rectangles from a list of list of \a bounds in the form ((x_lb_1, x_ub_1, y_lb_1, ...), (x_lb_2, x_ub_2, y_lb_2, ...), ...)
  VIBES_FUNC_COLOR_PARAM_1(drawBoxes,const std::vector< std::vector<double> > &,bounds)
  /// Computes and draw the union of a list of N-D rectangles, from a list of list of \a bounds in the form ((x_lb_1, x_ub_1, y_lb_1, ...), (x_lb_2, x_ub_2, y_lb_2, ...), ...)
  VIBES_FUNC_COLOR_PARAM_1(drawBoxesUnion,const std::vector< std::vector<double> > &,bounds)

  /// Draw a N-D line from the list of coordinates \a points in the form ((x_1, y_1, z_1, ...), (x_2, y_2, z_2, ...), ...)
  VIBES_FUNC_COLOR_PARAM_1(drawLine,const std::vector< std::vector<double> > &,points)
  /// Draw a 2-D line from the list of abscissae \a x and the list of ordinates \a y
  VIBES_FUNC_COLOR_PARAM_2(drawLine,const std::vector<double> &,x, const std::vector<double> &,y)

  /// @}
  /// @name Objects grouping and deletion
  /// @{

  /// Create a new group with the specified \a name.
  VIBES_FUNC_COLOR_PARAM_1(newGroup,const std::string &,name)

  /// Clear the contents of the group \a groupName in figure \a figureName.
  void clearGroup(const std::string &figureName, const std::string &groupName);
  /// Clear the contents of the group \a groupName in current figure.
  void clearGroup(const std::string &groupName);

  /// Delete the named graphical object \a objectName from figure \a figureName.
  void removeObject(const std::string &figureName, const std::string &objectName);
  /// Delete the named graphical object \a objectName from current figure.
  void removeObject(const std::string &objectName);

  /// @}
  /// @name Object properties modification
  /// @{

  /// Set the property \a key to the provided \a value for the named object \a objectName in figure \a figureName.
  void setObjectProperty(const std::string &figureName, const std::string &objectName, const std::string &key, const Value &value);
  /// Set the property \a key to the provided \a value for the named object \a objectName in current figure.
  void setObjectProperty(const std::string &objectName, const std::string &key, const Value &value);

  /// Assign \a properties to the named object \a objectName in figure \a figureName.
  void setObjectProperties(const std::string &figureName, const std::string &objectName, const Params &properties);
  /// Assign \a properties to the named object \a objectName in current figure.
  void setObjectProperties(const std::string &objectName, const Params &properties);

  /// @}
  ///
  ///

  /** @} */ // end of group drawing


  /** @addtogroup figure
   *  @{
   */

  ///
  /// @name Figure properties modification
  /// @{

  /// Set the property \a key to the provided \a value for figure \a figureName.
  void setFigureProperty(const std::string &figureName, const std::string &key, const Value &value);
  /// Set the property \a key to the provided \a value for current figure.
  void setFigureProperty(const std::string &key, const Value &value);

  /// Assign the given \a properties to the figure \a figureName.
  void setFigureProperties(const std::string &figureName, const Params &properties);
  /// Assign the given \a properties to the current figure.
  void setFigureProperties(const Params &properties);

  /// @}
  ///
  ///

  /** @} */ // end of group figure


  // Ibex enabled functions
  #ifdef _IBEX_INTERVAL_H_
    VIBES_FUNC_COLOR_PARAM_2(drawBox,const ibex::Interval &,x, const ibex::Interval &,y)
  #endif //#ifdef _IBEX_INTERVAL_H_
  #ifdef __IBEX_INTERVAL_VECTOR_H__
    VIBES_FUNC_COLOR_PARAM_1(drawBox,const ibex::IntervalVector &,box);
  #endif //#ifdef __IBEX_INTERVAL_VECTOR_H__


  //
  // Inline Implementations
  //

   /**
    * \brief Draw a circle
    * \param cx Abscissa of the center
    * \param cy Ordinate of the cnter
    * \param r Radius of the circle
    * \param params Optional attributes
    *
    * Draws a circle of radius \a r with center at (\a cx , \a cy ).
    * This functions internally calls \fn drawEllipse
    */
  inline void drawCircle(const double &cx, const double &cy, const double &r, const Params &params)
  {
     drawEllipse(cx,cy,r,r,0.,params);
  }

  /**
   * @brief Sets property \a key to the provided \a value for the figure named \a figureName.
   * @param figureName The name of the figure to alter
   * @param key        The name of the property to alter
   * @param value      The new value of the specified property
   */
  inline void setFigureProperty(const std::string &figureName, const std::string &key, const Value &value)
  {
     setFigureProperties(figureName, Params(key, value));
  }

  /**
   * Sets property \a key to the provided \a value for the current figure.
   * @overload
   */
  inline void setFigureProperty(const std::string &key, const Value &value)
  {
     setFigureProperties(Params(key, value));
  }

  inline void setObjectProperty(const std::string &figureName, const std::string &objectName, const std::string &key, const Value &value)
  {
     setObjectProperties(figureName, objectName, Params(key, value));
  }

  inline void setObjectProperty(const std::string &objectName, const std::string &key, const Value &value)
  {
     setObjectProperties(objectName, Params(key, value));
  }

  // Ibex enabled functions
  #ifdef _IBEX_INTERVAL_H_
    inline void drawBox(const ibex::Interval &x, const ibex::Interval &y, Params params) {
        drawBox(x.lb(),x.ub(),y.lb(),y.ub(),params);
    }
  #endif //#ifdef _IBEX_INTERVAL_H_
  #ifdef __IBEX_INTERVAL_VECTOR_H__
  /// \todo N-dimensionanl Ibex Inteval vector support
    inline void drawBox(const ibex::IntervalVector &box, Params params) {
        drawBox(box[0], box[1], params);
    }
  #endif //#ifdef __IBEX_INTERVAL_VECTOR_H__
}

///
/// Convenience macros
///
#ifdef VIBES_GENERATE_vibesXXX_MACROS
/// Macro to simplify contruction of a Params object
#define vibesParams(...) (vibes::Params(), __VA_ARGS__)
/// Convenience macros for "Matlab style" variadic calls
#define vibesDrawBox(x_lb,x_ub,y_lb,y_ub,...) vibes::drawBox(x_lb,x_ub,y_lb,y_ub,vibesParams(__VA_ARGS__))
#define vibesDrawEllipse(cx,cy,a,b,rot,...) vibes::drawEllipse(cx,cy,a,b,rot,vibesParams(__VA_ARGS__))
#define vibesDrawConfidenceEllipse(cx,cy,sxx,sxy,syy,K,...) vibes::drawConfidenceEllipse(cx,cy,sxx,sxy,syy,K,vibesParams(__VA_ARGS__))
#define vibesDrawCircle(cx,cy,r,...) vibes::drawCircle(cx,cy,r,vibesParams(__VA_ARGS__))
#endif //#ifdef VIBES_GENERATE_vibesXXX_MACROS

#endif //#ifndef VIBES_CPP_API_H
