/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __itkDeformationFieldInterpolatingTransform_h
#define __itkDeformationFieldInterpolatingTransform_h

#include <iostream>
#include "itkAdvancedTransform.h"
#include "itkExceptionObject.h"
#include "itkImage.h"
#include "itkVectorInterpolateImageFunction.h"
#include "itkVectorNearestNeighborInterpolateImageFunction.h"


namespace itk
{

  /** \brief Transform that interpolates a given deformation field
  *
  * A simple transform that allows the user to set a deformation field.
  * TransformPoint adds the displacement to the input point.
  * This transform does not support optimizers. Its Set/GetParameters
  * is not implemented. DO NOT USE IT FOR REGISTRATION.
  * You may set your own interpolator!
  *
  * \ingroup Transforms
  */

  template <
    class TScalarType=double,          // Data type for scalars (float or double)
    unsigned int NDimensions=3,        // Number of input dimensions
    class TComponentType=double>       // ComponentType of the deformation field
  class DeformationFieldInterpolatingTransform :
    public AdvancedTransform< TScalarType, NDimensions, NDimensions >
  {
  public:
    /** Standard class typedefs. */
    typedef DeformationFieldInterpolatingTransform Self;
    typedef AdvancedTransform< TScalarType, NDimensions, NDimensions > Superclass;
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    /** New macro for creation of through the object factory.*/
    itkNewMacro( Self );

    /** Run-time type information (and related methods). */
    itkTypeMacro( DeformationFieldInterpolatingTransform, AdvancedTransform );

    /** Dimension of the domain spaces. */
    itkStaticConstMacro(InputSpaceDimension, unsigned int, Superclass::InputSpaceDimension);
    itkStaticConstMacro(OutputSpaceDimension, unsigned int, Superclass::OutputSpaceDimension);

    /** Superclass typedefs */
    typedef typename Superclass::ScalarType           ScalarType;
    typedef typename Superclass::ParametersType       ParametersType;
    typedef typename Superclass::JacobianType         JacobianType;
    typedef typename Superclass::InputVectorType      InputVectorType;
    typedef typename Superclass::OutputVectorType     OutputVectorType;
    typedef typename Superclass::InputCovariantVectorType   InputCovariantVectorType;
    typedef typename Superclass::OutputCovariantVectorType  OutputCovariantVectorType;
    typedef typename Superclass::InputVnlVectorType   InputVnlVectorType;
    typedef typename Superclass::OutputVnlVectorType  OutputVnlVectorType;
    typedef typename Superclass::InputPointType       InputPointType;
    typedef typename Superclass::OutputPointType      OutputPointType;

    typedef TComponentType                            DeformationFieldComponentType;
    typedef Vector<DeformationFieldComponentType,
      itkGetStaticConstMacro(OutputSpaceDimension) >  DeformationFieldVectorType;
    typedef Image< DeformationFieldVectorType,
      itkGetStaticConstMacro(InputSpaceDimension) >   DeformationFieldType;

    typedef VectorInterpolateImageFunction<
      DeformationFieldType, ScalarType >              DeformationFieldInterpolatorType;
    typedef VectorNearestNeighborInterpolateImageFunction<
      DeformationFieldType, ScalarType >              DefaultDeformationFieldInterpolatorType;

    /** Set the transformation parameters is not supported.
     * Use SetDeformationField() instead
     */
    virtual void SetParameters( const ParametersType & )
    {
      itkExceptionMacro( << "ERROR: SetParameters() is not implemented "
        << "for DeformationFieldInterpolatingTransform.\n"
        << "Use SetDeformationField() instead.\n"
        << "Note that this transform is NOT suited for image registration.\n"
        << "Just use it as an (initial) fixed transform that is not optimized." );
    }

    /** Get the transformation parameters is not supported.
     * Use GetDeformationField() instead
     */
    virtual const ParametersType & GetParameters( void ) const
    {
      itkExceptionMacro( << "ERROR: GetParameters() is not implemented "
        << "for DeformationFieldInterpolatingTransform.\n"
        << "Use GetDeformationField() instead." );
    }

    /** Set the fixed parameters. */
    virtual void SetFixedParameters( const ParametersType & )
    {
      // This transform has no fixed parameters.
    }

    /** Get the Fixed Parameters. */
    virtual const ParametersType & GetFixedParameters( void ) const
    {
      // This transform has no fixed parameters.
      return m_FixedParameters;
    }

    /** Transform a point. This method adds a displacement to a given point,
     * returning the transformed point.
     */
    OutputPointType TransformPoint( const InputPointType & point ) const;

    /** These vector transforms are not implemented for this transform. */
    virtual OutputVectorType TransformVector( const InputVectorType & ) const
    {
      itkExceptionMacro(
        << "TransformVector(const InputVectorType &) is not implemented "
        << "for DeformationFieldInterpolatingTransform" );
    }
    virtual OutputVnlVectorType TransformVector( const InputVnlVectorType & ) const
    {
      itkExceptionMacro(
        << "TransformVector(const InputVnlVectorType &) is not implemented "
        << "for DeformationFieldInterpolatingTransform" );
    }
    virtual OutputCovariantVectorType TransformCovariantVector( const InputCovariantVectorType & ) const
    {
      itkExceptionMacro(
        << "TransformCovariantVector(const InputCovariantVectorType &) is not implemented "
        << "for DeformationFieldInterpolatingTransform" );
    }

    /** Make this an identity transform ( the deformation field is replaced
     * by a zero deformation field */
    void SetIdentity( void );

    /** Set/Get the deformation field that defines the displacements */
    virtual void SetDeformationField( DeformationFieldType * _arg );
    itkGetObjectMacro( DeformationField, DeformationFieldType );

    /** Set/Get the deformation field interpolator */
    virtual void SetDeformationFieldInterpolator( DeformationFieldInterpolatorType * _arg );
    itkGetObjectMacro( DeformationFieldInterpolator, DeformationFieldInterpolatorType );

    virtual bool IsLinear( void ) const { return false; };

    /** Must be provided. */
    virtual void GetJacobian( const InputPointType & ipp, JacobianType & j,
      NonZeroJacobianIndicesType & nonZeroJacobianIndices ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
    }
    virtual const JacobianType & GetJacobian( const InputPointType & ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
      // Next line is needed to avoid errors due to: "function must return a value".
      return this->m_Jacobian;
    }
    virtual void GetSpatialJacobian(
      const InputPointType & ipp, SpatialJacobianType & sj ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
    }
    virtual void GetSpatialHessian(
      const InputPointType & ipp, SpatialHessianType & sh ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
    }
    virtual void GetJacobianOfSpatialJacobian(
      const InputPointType & ipp, JacobianOfSpatialJacobianType & jsj,
      NonZeroJacobianIndicesType & nonZeroJacobianIndices ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
    }
    virtual void GetJacobianOfSpatialJacobian(
      const InputPointType & ipp, SpatialJacobianType & sj,
      JacobianOfSpatialJacobianType & jsj,
      NonZeroJacobianIndicesType & nonZeroJacobianIndices ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
    }
    virtual void GetJacobianOfSpatialHessian(
      const InputPointType & ipp, JacobianOfSpatialHessianType & jsh,
      NonZeroJacobianIndicesType & nonZeroJacobianIndices ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
    }
    virtual void GetJacobianOfSpatialHessian(
      const InputPointType & ipp, SpatialHessianType & sh,
      JacobianOfSpatialHessianType & jsh,
      NonZeroJacobianIndicesType & nonZeroJacobianIndices ) const
    {
      itkExceptionMacro( << "Not implemented for DeformationFieldInterpolatingTransform" );
    }

  protected:
    DeformationFieldInterpolatingTransform();
    ~DeformationFieldInterpolatingTransform();

    /** Typedef which is used internally */
    typedef typename DeformationFieldInterpolatorType::ContinuousIndexType
      InputContinuousIndexType;
    typedef typename DeformationFieldInterpolatorType::OutputType InterpolatorOutputType;

    /** Print contents of an DeformationFieldInterpolatingTransform. */
    void PrintSelf(std::ostream &os, Indent indent) const;

    typename DeformationFieldType::Pointer m_DeformationField;
    typename DeformationFieldType::Pointer m_ZeroDeformationField;
    typename DeformationFieldInterpolatorType::Pointer m_DeformationFieldInterpolator;

  private:
    DeformationFieldInterpolatingTransform(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

  }; //class DeformationFieldInterpolatingTransform


}  // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDeformationFieldInterpolatingTransform.txx"
#endif

#endif /* __itkDeformationFieldInterpolatingTransform_h */
