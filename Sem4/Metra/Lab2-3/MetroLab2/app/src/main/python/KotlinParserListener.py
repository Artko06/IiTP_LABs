# Generated from kotlin/KotlinParser.g4 by ANTLR 4.13.2
from antlr4 import *
if "." in __name__:
    from .KotlinParser import KotlinParser
else:
    from KotlinParser import KotlinParser

# This class defines a complete listener for a parse tree produced by KotlinParser.
class KotlinParserListener(ParseTreeListener):

    # Enter a parse tree produced by KotlinParser#kotlinFile.
    def enterKotlinFile(self, ctx:KotlinParser.KotlinFileContext):
        pass

    # Exit a parse tree produced by KotlinParser#kotlinFile.
    def exitKotlinFile(self, ctx:KotlinParser.KotlinFileContext):
        pass


    # Enter a parse tree produced by KotlinParser#script.
    def enterScript(self, ctx:KotlinParser.ScriptContext):
        pass

    # Exit a parse tree produced by KotlinParser#script.
    def exitScript(self, ctx:KotlinParser.ScriptContext):
        pass


    # Enter a parse tree produced by KotlinParser#preamble.
    def enterPreamble(self, ctx:KotlinParser.PreambleContext):
        pass

    # Exit a parse tree produced by KotlinParser#preamble.
    def exitPreamble(self, ctx:KotlinParser.PreambleContext):
        pass


    # Enter a parse tree produced by KotlinParser#fileAnnotations.
    def enterFileAnnotations(self, ctx:KotlinParser.FileAnnotationsContext):
        pass

    # Exit a parse tree produced by KotlinParser#fileAnnotations.
    def exitFileAnnotations(self, ctx:KotlinParser.FileAnnotationsContext):
        pass


    # Enter a parse tree produced by KotlinParser#fileAnnotation.
    def enterFileAnnotation(self, ctx:KotlinParser.FileAnnotationContext):
        pass

    # Exit a parse tree produced by KotlinParser#fileAnnotation.
    def exitFileAnnotation(self, ctx:KotlinParser.FileAnnotationContext):
        pass


    # Enter a parse tree produced by KotlinParser#packageHeader.
    def enterPackageHeader(self, ctx:KotlinParser.PackageHeaderContext):
        pass

    # Exit a parse tree produced by KotlinParser#packageHeader.
    def exitPackageHeader(self, ctx:KotlinParser.PackageHeaderContext):
        pass


    # Enter a parse tree produced by KotlinParser#importList.
    def enterImportList(self, ctx:KotlinParser.ImportListContext):
        pass

    # Exit a parse tree produced by KotlinParser#importList.
    def exitImportList(self, ctx:KotlinParser.ImportListContext):
        pass


    # Enter a parse tree produced by KotlinParser#importHeader.
    def enterImportHeader(self, ctx:KotlinParser.ImportHeaderContext):
        pass

    # Exit a parse tree produced by KotlinParser#importHeader.
    def exitImportHeader(self, ctx:KotlinParser.ImportHeaderContext):
        pass


    # Enter a parse tree produced by KotlinParser#importAlias.
    def enterImportAlias(self, ctx:KotlinParser.ImportAliasContext):
        pass

    # Exit a parse tree produced by KotlinParser#importAlias.
    def exitImportAlias(self, ctx:KotlinParser.ImportAliasContext):
        pass


    # Enter a parse tree produced by KotlinParser#topLevelObject.
    def enterTopLevelObject(self, ctx:KotlinParser.TopLevelObjectContext):
        pass

    # Exit a parse tree produced by KotlinParser#topLevelObject.
    def exitTopLevelObject(self, ctx:KotlinParser.TopLevelObjectContext):
        pass


    # Enter a parse tree produced by KotlinParser#classDeclaration.
    def enterClassDeclaration(self, ctx:KotlinParser.ClassDeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#classDeclaration.
    def exitClassDeclaration(self, ctx:KotlinParser.ClassDeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#primaryConstructor.
    def enterPrimaryConstructor(self, ctx:KotlinParser.PrimaryConstructorContext):
        pass

    # Exit a parse tree produced by KotlinParser#primaryConstructor.
    def exitPrimaryConstructor(self, ctx:KotlinParser.PrimaryConstructorContext):
        pass


    # Enter a parse tree produced by KotlinParser#classParameters.
    def enterClassParameters(self, ctx:KotlinParser.ClassParametersContext):
        pass

    # Exit a parse tree produced by KotlinParser#classParameters.
    def exitClassParameters(self, ctx:KotlinParser.ClassParametersContext):
        pass


    # Enter a parse tree produced by KotlinParser#classParameter.
    def enterClassParameter(self, ctx:KotlinParser.ClassParameterContext):
        pass

    # Exit a parse tree produced by KotlinParser#classParameter.
    def exitClassParameter(self, ctx:KotlinParser.ClassParameterContext):
        pass


    # Enter a parse tree produced by KotlinParser#delegationSpecifiers.
    def enterDelegationSpecifiers(self, ctx:KotlinParser.DelegationSpecifiersContext):
        pass

    # Exit a parse tree produced by KotlinParser#delegationSpecifiers.
    def exitDelegationSpecifiers(self, ctx:KotlinParser.DelegationSpecifiersContext):
        pass


    # Enter a parse tree produced by KotlinParser#delegationSpecifier.
    def enterDelegationSpecifier(self, ctx:KotlinParser.DelegationSpecifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#delegationSpecifier.
    def exitDelegationSpecifier(self, ctx:KotlinParser.DelegationSpecifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#constructorInvocation.
    def enterConstructorInvocation(self, ctx:KotlinParser.ConstructorInvocationContext):
        pass

    # Exit a parse tree produced by KotlinParser#constructorInvocation.
    def exitConstructorInvocation(self, ctx:KotlinParser.ConstructorInvocationContext):
        pass


    # Enter a parse tree produced by KotlinParser#explicitDelegation.
    def enterExplicitDelegation(self, ctx:KotlinParser.ExplicitDelegationContext):
        pass

    # Exit a parse tree produced by KotlinParser#explicitDelegation.
    def exitExplicitDelegation(self, ctx:KotlinParser.ExplicitDelegationContext):
        pass


    # Enter a parse tree produced by KotlinParser#classBody.
    def enterClassBody(self, ctx:KotlinParser.ClassBodyContext):
        pass

    # Exit a parse tree produced by KotlinParser#classBody.
    def exitClassBody(self, ctx:KotlinParser.ClassBodyContext):
        pass


    # Enter a parse tree produced by KotlinParser#classMemberDeclaration.
    def enterClassMemberDeclaration(self, ctx:KotlinParser.ClassMemberDeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#classMemberDeclaration.
    def exitClassMemberDeclaration(self, ctx:KotlinParser.ClassMemberDeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#anonymousInitializer.
    def enterAnonymousInitializer(self, ctx:KotlinParser.AnonymousInitializerContext):
        pass

    # Exit a parse tree produced by KotlinParser#anonymousInitializer.
    def exitAnonymousInitializer(self, ctx:KotlinParser.AnonymousInitializerContext):
        pass


    # Enter a parse tree produced by KotlinParser#secondaryConstructor.
    def enterSecondaryConstructor(self, ctx:KotlinParser.SecondaryConstructorContext):
        pass

    # Exit a parse tree produced by KotlinParser#secondaryConstructor.
    def exitSecondaryConstructor(self, ctx:KotlinParser.SecondaryConstructorContext):
        pass


    # Enter a parse tree produced by KotlinParser#constructorDelegationCall.
    def enterConstructorDelegationCall(self, ctx:KotlinParser.ConstructorDelegationCallContext):
        pass

    # Exit a parse tree produced by KotlinParser#constructorDelegationCall.
    def exitConstructorDelegationCall(self, ctx:KotlinParser.ConstructorDelegationCallContext):
        pass


    # Enter a parse tree produced by KotlinParser#enumClassBody.
    def enterEnumClassBody(self, ctx:KotlinParser.EnumClassBodyContext):
        pass

    # Exit a parse tree produced by KotlinParser#enumClassBody.
    def exitEnumClassBody(self, ctx:KotlinParser.EnumClassBodyContext):
        pass


    # Enter a parse tree produced by KotlinParser#enumEntries.
    def enterEnumEntries(self, ctx:KotlinParser.EnumEntriesContext):
        pass

    # Exit a parse tree produced by KotlinParser#enumEntries.
    def exitEnumEntries(self, ctx:KotlinParser.EnumEntriesContext):
        pass


    # Enter a parse tree produced by KotlinParser#enumEntry.
    def enterEnumEntry(self, ctx:KotlinParser.EnumEntryContext):
        pass

    # Exit a parse tree produced by KotlinParser#enumEntry.
    def exitEnumEntry(self, ctx:KotlinParser.EnumEntryContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionDeclaration.
    def enterFunctionDeclaration(self, ctx:KotlinParser.FunctionDeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionDeclaration.
    def exitFunctionDeclaration(self, ctx:KotlinParser.FunctionDeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionValueParameters.
    def enterFunctionValueParameters(self, ctx:KotlinParser.FunctionValueParametersContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionValueParameters.
    def exitFunctionValueParameters(self, ctx:KotlinParser.FunctionValueParametersContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionValueParameter.
    def enterFunctionValueParameter(self, ctx:KotlinParser.FunctionValueParameterContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionValueParameter.
    def exitFunctionValueParameter(self, ctx:KotlinParser.FunctionValueParameterContext):
        pass


    # Enter a parse tree produced by KotlinParser#parameter.
    def enterParameter(self, ctx:KotlinParser.ParameterContext):
        pass

    # Exit a parse tree produced by KotlinParser#parameter.
    def exitParameter(self, ctx:KotlinParser.ParameterContext):
        pass


    # Enter a parse tree produced by KotlinParser#receiverType.
    def enterReceiverType(self, ctx:KotlinParser.ReceiverTypeContext):
        pass

    # Exit a parse tree produced by KotlinParser#receiverType.
    def exitReceiverType(self, ctx:KotlinParser.ReceiverTypeContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionBody.
    def enterFunctionBody(self, ctx:KotlinParser.FunctionBodyContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionBody.
    def exitFunctionBody(self, ctx:KotlinParser.FunctionBodyContext):
        pass


    # Enter a parse tree produced by KotlinParser#objectDeclaration.
    def enterObjectDeclaration(self, ctx:KotlinParser.ObjectDeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#objectDeclaration.
    def exitObjectDeclaration(self, ctx:KotlinParser.ObjectDeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#companionObject.
    def enterCompanionObject(self, ctx:KotlinParser.CompanionObjectContext):
        pass

    # Exit a parse tree produced by KotlinParser#companionObject.
    def exitCompanionObject(self, ctx:KotlinParser.CompanionObjectContext):
        pass


    # Enter a parse tree produced by KotlinParser#propertyDeclaration.
    def enterPropertyDeclaration(self, ctx:KotlinParser.PropertyDeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#propertyDeclaration.
    def exitPropertyDeclaration(self, ctx:KotlinParser.PropertyDeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#multiVariableDeclaration.
    def enterMultiVariableDeclaration(self, ctx:KotlinParser.MultiVariableDeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#multiVariableDeclaration.
    def exitMultiVariableDeclaration(self, ctx:KotlinParser.MultiVariableDeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#variableDeclaration.
    def enterVariableDeclaration(self, ctx:KotlinParser.VariableDeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#variableDeclaration.
    def exitVariableDeclaration(self, ctx:KotlinParser.VariableDeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#getter.
    def enterGetter(self, ctx:KotlinParser.GetterContext):
        pass

    # Exit a parse tree produced by KotlinParser#getter.
    def exitGetter(self, ctx:KotlinParser.GetterContext):
        pass


    # Enter a parse tree produced by KotlinParser#setter.
    def enterSetter(self, ctx:KotlinParser.SetterContext):
        pass

    # Exit a parse tree produced by KotlinParser#setter.
    def exitSetter(self, ctx:KotlinParser.SetterContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeAlias.
    def enterTypeAlias(self, ctx:KotlinParser.TypeAliasContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeAlias.
    def exitTypeAlias(self, ctx:KotlinParser.TypeAliasContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeParameters.
    def enterTypeParameters(self, ctx:KotlinParser.TypeParametersContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeParameters.
    def exitTypeParameters(self, ctx:KotlinParser.TypeParametersContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeParameter.
    def enterTypeParameter(self, ctx:KotlinParser.TypeParameterContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeParameter.
    def exitTypeParameter(self, ctx:KotlinParser.TypeParameterContext):
        pass


    # Enter a parse tree produced by KotlinParser#type.
    def enterType(self, ctx:KotlinParser.TypeContext):
        pass

    # Exit a parse tree produced by KotlinParser#type.
    def exitType(self, ctx:KotlinParser.TypeContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeModifierList.
    def enterTypeModifierList(self, ctx:KotlinParser.TypeModifierListContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeModifierList.
    def exitTypeModifierList(self, ctx:KotlinParser.TypeModifierListContext):
        pass


    # Enter a parse tree produced by KotlinParser#parenthesizedType.
    def enterParenthesizedType(self, ctx:KotlinParser.ParenthesizedTypeContext):
        pass

    # Exit a parse tree produced by KotlinParser#parenthesizedType.
    def exitParenthesizedType(self, ctx:KotlinParser.ParenthesizedTypeContext):
        pass


    # Enter a parse tree produced by KotlinParser#nullableType.
    def enterNullableType(self, ctx:KotlinParser.NullableTypeContext):
        pass

    # Exit a parse tree produced by KotlinParser#nullableType.
    def exitNullableType(self, ctx:KotlinParser.NullableTypeContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeReference.
    def enterTypeReference(self, ctx:KotlinParser.TypeReferenceContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeReference.
    def exitTypeReference(self, ctx:KotlinParser.TypeReferenceContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionType.
    def enterFunctionType(self, ctx:KotlinParser.FunctionTypeContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionType.
    def exitFunctionType(self, ctx:KotlinParser.FunctionTypeContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionTypeReceiver.
    def enterFunctionTypeReceiver(self, ctx:KotlinParser.FunctionTypeReceiverContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionTypeReceiver.
    def exitFunctionTypeReceiver(self, ctx:KotlinParser.FunctionTypeReceiverContext):
        pass


    # Enter a parse tree produced by KotlinParser#userType.
    def enterUserType(self, ctx:KotlinParser.UserTypeContext):
        pass

    # Exit a parse tree produced by KotlinParser#userType.
    def exitUserType(self, ctx:KotlinParser.UserTypeContext):
        pass


    # Enter a parse tree produced by KotlinParser#simpleUserType.
    def enterSimpleUserType(self, ctx:KotlinParser.SimpleUserTypeContext):
        pass

    # Exit a parse tree produced by KotlinParser#simpleUserType.
    def exitSimpleUserType(self, ctx:KotlinParser.SimpleUserTypeContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionTypeParameters.
    def enterFunctionTypeParameters(self, ctx:KotlinParser.FunctionTypeParametersContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionTypeParameters.
    def exitFunctionTypeParameters(self, ctx:KotlinParser.FunctionTypeParametersContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeConstraints.
    def enterTypeConstraints(self, ctx:KotlinParser.TypeConstraintsContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeConstraints.
    def exitTypeConstraints(self, ctx:KotlinParser.TypeConstraintsContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeConstraint.
    def enterTypeConstraint(self, ctx:KotlinParser.TypeConstraintContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeConstraint.
    def exitTypeConstraint(self, ctx:KotlinParser.TypeConstraintContext):
        pass


    # Enter a parse tree produced by KotlinParser#block.
    def enterBlock(self, ctx:KotlinParser.BlockContext):
        pass

    # Exit a parse tree produced by KotlinParser#block.
    def exitBlock(self, ctx:KotlinParser.BlockContext):
        pass


    # Enter a parse tree produced by KotlinParser#statements.
    def enterStatements(self, ctx:KotlinParser.StatementsContext):
        pass

    # Exit a parse tree produced by KotlinParser#statements.
    def exitStatements(self, ctx:KotlinParser.StatementsContext):
        pass


    # Enter a parse tree produced by KotlinParser#statement.
    def enterStatement(self, ctx:KotlinParser.StatementContext):
        pass

    # Exit a parse tree produced by KotlinParser#statement.
    def exitStatement(self, ctx:KotlinParser.StatementContext):
        pass


    # Enter a parse tree produced by KotlinParser#blockLevelExpression.
    def enterBlockLevelExpression(self, ctx:KotlinParser.BlockLevelExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#blockLevelExpression.
    def exitBlockLevelExpression(self, ctx:KotlinParser.BlockLevelExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#declaration.
    def enterDeclaration(self, ctx:KotlinParser.DeclarationContext):
        pass

    # Exit a parse tree produced by KotlinParser#declaration.
    def exitDeclaration(self, ctx:KotlinParser.DeclarationContext):
        pass


    # Enter a parse tree produced by KotlinParser#expression.
    def enterExpression(self, ctx:KotlinParser.ExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#expression.
    def exitExpression(self, ctx:KotlinParser.ExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#disjunction.
    def enterDisjunction(self, ctx:KotlinParser.DisjunctionContext):
        pass

    # Exit a parse tree produced by KotlinParser#disjunction.
    def exitDisjunction(self, ctx:KotlinParser.DisjunctionContext):
        pass


    # Enter a parse tree produced by KotlinParser#conjunction.
    def enterConjunction(self, ctx:KotlinParser.ConjunctionContext):
        pass

    # Exit a parse tree produced by KotlinParser#conjunction.
    def exitConjunction(self, ctx:KotlinParser.ConjunctionContext):
        pass


    # Enter a parse tree produced by KotlinParser#equalityComparison.
    def enterEqualityComparison(self, ctx:KotlinParser.EqualityComparisonContext):
        pass

    # Exit a parse tree produced by KotlinParser#equalityComparison.
    def exitEqualityComparison(self, ctx:KotlinParser.EqualityComparisonContext):
        pass


    # Enter a parse tree produced by KotlinParser#comparison.
    def enterComparison(self, ctx:KotlinParser.ComparisonContext):
        pass

    # Exit a parse tree produced by KotlinParser#comparison.
    def exitComparison(self, ctx:KotlinParser.ComparisonContext):
        pass


    # Enter a parse tree produced by KotlinParser#namedInfix.
    def enterNamedInfix(self, ctx:KotlinParser.NamedInfixContext):
        pass

    # Exit a parse tree produced by KotlinParser#namedInfix.
    def exitNamedInfix(self, ctx:KotlinParser.NamedInfixContext):
        pass


    # Enter a parse tree produced by KotlinParser#elvisExpression.
    def enterElvisExpression(self, ctx:KotlinParser.ElvisExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#elvisExpression.
    def exitElvisExpression(self, ctx:KotlinParser.ElvisExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#infixFunctionCall.
    def enterInfixFunctionCall(self, ctx:KotlinParser.InfixFunctionCallContext):
        pass

    # Exit a parse tree produced by KotlinParser#infixFunctionCall.
    def exitInfixFunctionCall(self, ctx:KotlinParser.InfixFunctionCallContext):
        pass


    # Enter a parse tree produced by KotlinParser#rangeExpression.
    def enterRangeExpression(self, ctx:KotlinParser.RangeExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#rangeExpression.
    def exitRangeExpression(self, ctx:KotlinParser.RangeExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#additiveExpression.
    def enterAdditiveExpression(self, ctx:KotlinParser.AdditiveExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#additiveExpression.
    def exitAdditiveExpression(self, ctx:KotlinParser.AdditiveExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#multiplicativeExpression.
    def enterMultiplicativeExpression(self, ctx:KotlinParser.MultiplicativeExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#multiplicativeExpression.
    def exitMultiplicativeExpression(self, ctx:KotlinParser.MultiplicativeExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeRHS.
    def enterTypeRHS(self, ctx:KotlinParser.TypeRHSContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeRHS.
    def exitTypeRHS(self, ctx:KotlinParser.TypeRHSContext):
        pass


    # Enter a parse tree produced by KotlinParser#prefixUnaryExpression.
    def enterPrefixUnaryExpression(self, ctx:KotlinParser.PrefixUnaryExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#prefixUnaryExpression.
    def exitPrefixUnaryExpression(self, ctx:KotlinParser.PrefixUnaryExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#postfixUnaryExpression.
    def enterPostfixUnaryExpression(self, ctx:KotlinParser.PostfixUnaryExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#postfixUnaryExpression.
    def exitPostfixUnaryExpression(self, ctx:KotlinParser.PostfixUnaryExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#atomicExpression.
    def enterAtomicExpression(self, ctx:KotlinParser.AtomicExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#atomicExpression.
    def exitAtomicExpression(self, ctx:KotlinParser.AtomicExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#parenthesizedExpression.
    def enterParenthesizedExpression(self, ctx:KotlinParser.ParenthesizedExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#parenthesizedExpression.
    def exitParenthesizedExpression(self, ctx:KotlinParser.ParenthesizedExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#callSuffix.
    def enterCallSuffix(self, ctx:KotlinParser.CallSuffixContext):
        pass

    # Exit a parse tree produced by KotlinParser#callSuffix.
    def exitCallSuffix(self, ctx:KotlinParser.CallSuffixContext):
        pass


    # Enter a parse tree produced by KotlinParser#annotatedLambda.
    def enterAnnotatedLambda(self, ctx:KotlinParser.AnnotatedLambdaContext):
        pass

    # Exit a parse tree produced by KotlinParser#annotatedLambda.
    def exitAnnotatedLambda(self, ctx:KotlinParser.AnnotatedLambdaContext):
        pass


    # Enter a parse tree produced by KotlinParser#arrayAccess.
    def enterArrayAccess(self, ctx:KotlinParser.ArrayAccessContext):
        pass

    # Exit a parse tree produced by KotlinParser#arrayAccess.
    def exitArrayAccess(self, ctx:KotlinParser.ArrayAccessContext):
        pass


    # Enter a parse tree produced by KotlinParser#valueArguments.
    def enterValueArguments(self, ctx:KotlinParser.ValueArgumentsContext):
        pass

    # Exit a parse tree produced by KotlinParser#valueArguments.
    def exitValueArguments(self, ctx:KotlinParser.ValueArgumentsContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeArguments.
    def enterTypeArguments(self, ctx:KotlinParser.TypeArgumentsContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeArguments.
    def exitTypeArguments(self, ctx:KotlinParser.TypeArgumentsContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeProjection.
    def enterTypeProjection(self, ctx:KotlinParser.TypeProjectionContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeProjection.
    def exitTypeProjection(self, ctx:KotlinParser.TypeProjectionContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeProjectionModifierList.
    def enterTypeProjectionModifierList(self, ctx:KotlinParser.TypeProjectionModifierListContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeProjectionModifierList.
    def exitTypeProjectionModifierList(self, ctx:KotlinParser.TypeProjectionModifierListContext):
        pass


    # Enter a parse tree produced by KotlinParser#valueArgument.
    def enterValueArgument(self, ctx:KotlinParser.ValueArgumentContext):
        pass

    # Exit a parse tree produced by KotlinParser#valueArgument.
    def exitValueArgument(self, ctx:KotlinParser.ValueArgumentContext):
        pass


    # Enter a parse tree produced by KotlinParser#literalConstant.
    def enterLiteralConstant(self, ctx:KotlinParser.LiteralConstantContext):
        pass

    # Exit a parse tree produced by KotlinParser#literalConstant.
    def exitLiteralConstant(self, ctx:KotlinParser.LiteralConstantContext):
        pass


    # Enter a parse tree produced by KotlinParser#stringLiteral.
    def enterStringLiteral(self, ctx:KotlinParser.StringLiteralContext):
        pass

    # Exit a parse tree produced by KotlinParser#stringLiteral.
    def exitStringLiteral(self, ctx:KotlinParser.StringLiteralContext):
        pass


    # Enter a parse tree produced by KotlinParser#lineStringLiteral.
    def enterLineStringLiteral(self, ctx:KotlinParser.LineStringLiteralContext):
        pass

    # Exit a parse tree produced by KotlinParser#lineStringLiteral.
    def exitLineStringLiteral(self, ctx:KotlinParser.LineStringLiteralContext):
        pass


    # Enter a parse tree produced by KotlinParser#multiLineStringLiteral.
    def enterMultiLineStringLiteral(self, ctx:KotlinParser.MultiLineStringLiteralContext):
        pass

    # Exit a parse tree produced by KotlinParser#multiLineStringLiteral.
    def exitMultiLineStringLiteral(self, ctx:KotlinParser.MultiLineStringLiteralContext):
        pass


    # Enter a parse tree produced by KotlinParser#lineStringContent.
    def enterLineStringContent(self, ctx:KotlinParser.LineStringContentContext):
        pass

    # Exit a parse tree produced by KotlinParser#lineStringContent.
    def exitLineStringContent(self, ctx:KotlinParser.LineStringContentContext):
        pass


    # Enter a parse tree produced by KotlinParser#lineStringExpression.
    def enterLineStringExpression(self, ctx:KotlinParser.LineStringExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#lineStringExpression.
    def exitLineStringExpression(self, ctx:KotlinParser.LineStringExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#multiLineStringContent.
    def enterMultiLineStringContent(self, ctx:KotlinParser.MultiLineStringContentContext):
        pass

    # Exit a parse tree produced by KotlinParser#multiLineStringContent.
    def exitMultiLineStringContent(self, ctx:KotlinParser.MultiLineStringContentContext):
        pass


    # Enter a parse tree produced by KotlinParser#multiLineStringExpression.
    def enterMultiLineStringExpression(self, ctx:KotlinParser.MultiLineStringExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#multiLineStringExpression.
    def exitMultiLineStringExpression(self, ctx:KotlinParser.MultiLineStringExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionLiteral.
    def enterFunctionLiteral(self, ctx:KotlinParser.FunctionLiteralContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionLiteral.
    def exitFunctionLiteral(self, ctx:KotlinParser.FunctionLiteralContext):
        pass


    # Enter a parse tree produced by KotlinParser#lambdaParameters.
    def enterLambdaParameters(self, ctx:KotlinParser.LambdaParametersContext):
        pass

    # Exit a parse tree produced by KotlinParser#lambdaParameters.
    def exitLambdaParameters(self, ctx:KotlinParser.LambdaParametersContext):
        pass


    # Enter a parse tree produced by KotlinParser#lambdaParameter.
    def enterLambdaParameter(self, ctx:KotlinParser.LambdaParameterContext):
        pass

    # Exit a parse tree produced by KotlinParser#lambdaParameter.
    def exitLambdaParameter(self, ctx:KotlinParser.LambdaParameterContext):
        pass


    # Enter a parse tree produced by KotlinParser#objectLiteral.
    def enterObjectLiteral(self, ctx:KotlinParser.ObjectLiteralContext):
        pass

    # Exit a parse tree produced by KotlinParser#objectLiteral.
    def exitObjectLiteral(self, ctx:KotlinParser.ObjectLiteralContext):
        pass


    # Enter a parse tree produced by KotlinParser#collectionLiteral.
    def enterCollectionLiteral(self, ctx:KotlinParser.CollectionLiteralContext):
        pass

    # Exit a parse tree produced by KotlinParser#collectionLiteral.
    def exitCollectionLiteral(self, ctx:KotlinParser.CollectionLiteralContext):
        pass


    # Enter a parse tree produced by KotlinParser#thisExpression.
    def enterThisExpression(self, ctx:KotlinParser.ThisExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#thisExpression.
    def exitThisExpression(self, ctx:KotlinParser.ThisExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#superExpression.
    def enterSuperExpression(self, ctx:KotlinParser.SuperExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#superExpression.
    def exitSuperExpression(self, ctx:KotlinParser.SuperExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#conditionalExpression.
    def enterConditionalExpression(self, ctx:KotlinParser.ConditionalExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#conditionalExpression.
    def exitConditionalExpression(self, ctx:KotlinParser.ConditionalExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#ifExpression.
    def enterIfExpression(self, ctx:KotlinParser.IfExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#ifExpression.
    def exitIfExpression(self, ctx:KotlinParser.IfExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#controlStructureBody.
    def enterControlStructureBody(self, ctx:KotlinParser.ControlStructureBodyContext):
        pass

    # Exit a parse tree produced by KotlinParser#controlStructureBody.
    def exitControlStructureBody(self, ctx:KotlinParser.ControlStructureBodyContext):
        pass


    # Enter a parse tree produced by KotlinParser#whenExpression.
    def enterWhenExpression(self, ctx:KotlinParser.WhenExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#whenExpression.
    def exitWhenExpression(self, ctx:KotlinParser.WhenExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#whenEntry.
    def enterWhenEntry(self, ctx:KotlinParser.WhenEntryContext):
        pass

    # Exit a parse tree produced by KotlinParser#whenEntry.
    def exitWhenEntry(self, ctx:KotlinParser.WhenEntryContext):
        pass


    # Enter a parse tree produced by KotlinParser#whenCondition.
    def enterWhenCondition(self, ctx:KotlinParser.WhenConditionContext):
        pass

    # Exit a parse tree produced by KotlinParser#whenCondition.
    def exitWhenCondition(self, ctx:KotlinParser.WhenConditionContext):
        pass


    # Enter a parse tree produced by KotlinParser#rangeTest.
    def enterRangeTest(self, ctx:KotlinParser.RangeTestContext):
        pass

    # Exit a parse tree produced by KotlinParser#rangeTest.
    def exitRangeTest(self, ctx:KotlinParser.RangeTestContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeTest.
    def enterTypeTest(self, ctx:KotlinParser.TypeTestContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeTest.
    def exitTypeTest(self, ctx:KotlinParser.TypeTestContext):
        pass


    # Enter a parse tree produced by KotlinParser#tryExpression.
    def enterTryExpression(self, ctx:KotlinParser.TryExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#tryExpression.
    def exitTryExpression(self, ctx:KotlinParser.TryExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#catchBlock.
    def enterCatchBlock(self, ctx:KotlinParser.CatchBlockContext):
        pass

    # Exit a parse tree produced by KotlinParser#catchBlock.
    def exitCatchBlock(self, ctx:KotlinParser.CatchBlockContext):
        pass


    # Enter a parse tree produced by KotlinParser#finallyBlock.
    def enterFinallyBlock(self, ctx:KotlinParser.FinallyBlockContext):
        pass

    # Exit a parse tree produced by KotlinParser#finallyBlock.
    def exitFinallyBlock(self, ctx:KotlinParser.FinallyBlockContext):
        pass


    # Enter a parse tree produced by KotlinParser#loopExpression.
    def enterLoopExpression(self, ctx:KotlinParser.LoopExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#loopExpression.
    def exitLoopExpression(self, ctx:KotlinParser.LoopExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#forExpression.
    def enterForExpression(self, ctx:KotlinParser.ForExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#forExpression.
    def exitForExpression(self, ctx:KotlinParser.ForExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#whileExpression.
    def enterWhileExpression(self, ctx:KotlinParser.WhileExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#whileExpression.
    def exitWhileExpression(self, ctx:KotlinParser.WhileExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#doWhileExpression.
    def enterDoWhileExpression(self, ctx:KotlinParser.DoWhileExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#doWhileExpression.
    def exitDoWhileExpression(self, ctx:KotlinParser.DoWhileExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#jumpExpression.
    def enterJumpExpression(self, ctx:KotlinParser.JumpExpressionContext):
        pass

    # Exit a parse tree produced by KotlinParser#jumpExpression.
    def exitJumpExpression(self, ctx:KotlinParser.JumpExpressionContext):
        pass


    # Enter a parse tree produced by KotlinParser#callableReference.
    def enterCallableReference(self, ctx:KotlinParser.CallableReferenceContext):
        pass

    # Exit a parse tree produced by KotlinParser#callableReference.
    def exitCallableReference(self, ctx:KotlinParser.CallableReferenceContext):
        pass


    # Enter a parse tree produced by KotlinParser#assignmentOperator.
    def enterAssignmentOperator(self, ctx:KotlinParser.AssignmentOperatorContext):
        pass

    # Exit a parse tree produced by KotlinParser#assignmentOperator.
    def exitAssignmentOperator(self, ctx:KotlinParser.AssignmentOperatorContext):
        pass


    # Enter a parse tree produced by KotlinParser#equalityOperation.
    def enterEqualityOperation(self, ctx:KotlinParser.EqualityOperationContext):
        pass

    # Exit a parse tree produced by KotlinParser#equalityOperation.
    def exitEqualityOperation(self, ctx:KotlinParser.EqualityOperationContext):
        pass


    # Enter a parse tree produced by KotlinParser#comparisonOperator.
    def enterComparisonOperator(self, ctx:KotlinParser.ComparisonOperatorContext):
        pass

    # Exit a parse tree produced by KotlinParser#comparisonOperator.
    def exitComparisonOperator(self, ctx:KotlinParser.ComparisonOperatorContext):
        pass


    # Enter a parse tree produced by KotlinParser#inOperator.
    def enterInOperator(self, ctx:KotlinParser.InOperatorContext):
        pass

    # Exit a parse tree produced by KotlinParser#inOperator.
    def exitInOperator(self, ctx:KotlinParser.InOperatorContext):
        pass


    # Enter a parse tree produced by KotlinParser#isOperator.
    def enterIsOperator(self, ctx:KotlinParser.IsOperatorContext):
        pass

    # Exit a parse tree produced by KotlinParser#isOperator.
    def exitIsOperator(self, ctx:KotlinParser.IsOperatorContext):
        pass


    # Enter a parse tree produced by KotlinParser#additiveOperator.
    def enterAdditiveOperator(self, ctx:KotlinParser.AdditiveOperatorContext):
        pass

    # Exit a parse tree produced by KotlinParser#additiveOperator.
    def exitAdditiveOperator(self, ctx:KotlinParser.AdditiveOperatorContext):
        pass


    # Enter a parse tree produced by KotlinParser#multiplicativeOperation.
    def enterMultiplicativeOperation(self, ctx:KotlinParser.MultiplicativeOperationContext):
        pass

    # Exit a parse tree produced by KotlinParser#multiplicativeOperation.
    def exitMultiplicativeOperation(self, ctx:KotlinParser.MultiplicativeOperationContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeOperation.
    def enterTypeOperation(self, ctx:KotlinParser.TypeOperationContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeOperation.
    def exitTypeOperation(self, ctx:KotlinParser.TypeOperationContext):
        pass


    # Enter a parse tree produced by KotlinParser#prefixUnaryOperation.
    def enterPrefixUnaryOperation(self, ctx:KotlinParser.PrefixUnaryOperationContext):
        pass

    # Exit a parse tree produced by KotlinParser#prefixUnaryOperation.
    def exitPrefixUnaryOperation(self, ctx:KotlinParser.PrefixUnaryOperationContext):
        pass


    # Enter a parse tree produced by KotlinParser#postfixUnaryOperation.
    def enterPostfixUnaryOperation(self, ctx:KotlinParser.PostfixUnaryOperationContext):
        pass

    # Exit a parse tree produced by KotlinParser#postfixUnaryOperation.
    def exitPostfixUnaryOperation(self, ctx:KotlinParser.PostfixUnaryOperationContext):
        pass


    # Enter a parse tree produced by KotlinParser#memberAccessOperator.
    def enterMemberAccessOperator(self, ctx:KotlinParser.MemberAccessOperatorContext):
        pass

    # Exit a parse tree produced by KotlinParser#memberAccessOperator.
    def exitMemberAccessOperator(self, ctx:KotlinParser.MemberAccessOperatorContext):
        pass


    # Enter a parse tree produced by KotlinParser#modifierList.
    def enterModifierList(self, ctx:KotlinParser.ModifierListContext):
        pass

    # Exit a parse tree produced by KotlinParser#modifierList.
    def exitModifierList(self, ctx:KotlinParser.ModifierListContext):
        pass


    # Enter a parse tree produced by KotlinParser#modifier.
    def enterModifier(self, ctx:KotlinParser.ModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#modifier.
    def exitModifier(self, ctx:KotlinParser.ModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#classModifier.
    def enterClassModifier(self, ctx:KotlinParser.ClassModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#classModifier.
    def exitClassModifier(self, ctx:KotlinParser.ClassModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#memberModifier.
    def enterMemberModifier(self, ctx:KotlinParser.MemberModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#memberModifier.
    def exitMemberModifier(self, ctx:KotlinParser.MemberModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#visibilityModifier.
    def enterVisibilityModifier(self, ctx:KotlinParser.VisibilityModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#visibilityModifier.
    def exitVisibilityModifier(self, ctx:KotlinParser.VisibilityModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#varianceAnnotation.
    def enterVarianceAnnotation(self, ctx:KotlinParser.VarianceAnnotationContext):
        pass

    # Exit a parse tree produced by KotlinParser#varianceAnnotation.
    def exitVarianceAnnotation(self, ctx:KotlinParser.VarianceAnnotationContext):
        pass


    # Enter a parse tree produced by KotlinParser#functionModifier.
    def enterFunctionModifier(self, ctx:KotlinParser.FunctionModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#functionModifier.
    def exitFunctionModifier(self, ctx:KotlinParser.FunctionModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#propertyModifier.
    def enterPropertyModifier(self, ctx:KotlinParser.PropertyModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#propertyModifier.
    def exitPropertyModifier(self, ctx:KotlinParser.PropertyModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#inheritanceModifier.
    def enterInheritanceModifier(self, ctx:KotlinParser.InheritanceModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#inheritanceModifier.
    def exitInheritanceModifier(self, ctx:KotlinParser.InheritanceModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#parameterModifier.
    def enterParameterModifier(self, ctx:KotlinParser.ParameterModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#parameterModifier.
    def exitParameterModifier(self, ctx:KotlinParser.ParameterModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#typeParameterModifier.
    def enterTypeParameterModifier(self, ctx:KotlinParser.TypeParameterModifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#typeParameterModifier.
    def exitTypeParameterModifier(self, ctx:KotlinParser.TypeParameterModifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#labelDefinition.
    def enterLabelDefinition(self, ctx:KotlinParser.LabelDefinitionContext):
        pass

    # Exit a parse tree produced by KotlinParser#labelDefinition.
    def exitLabelDefinition(self, ctx:KotlinParser.LabelDefinitionContext):
        pass


    # Enter a parse tree produced by KotlinParser#annotations.
    def enterAnnotations(self, ctx:KotlinParser.AnnotationsContext):
        pass

    # Exit a parse tree produced by KotlinParser#annotations.
    def exitAnnotations(self, ctx:KotlinParser.AnnotationsContext):
        pass


    # Enter a parse tree produced by KotlinParser#annotation.
    def enterAnnotation(self, ctx:KotlinParser.AnnotationContext):
        pass

    # Exit a parse tree produced by KotlinParser#annotation.
    def exitAnnotation(self, ctx:KotlinParser.AnnotationContext):
        pass


    # Enter a parse tree produced by KotlinParser#annotationList.
    def enterAnnotationList(self, ctx:KotlinParser.AnnotationListContext):
        pass

    # Exit a parse tree produced by KotlinParser#annotationList.
    def exitAnnotationList(self, ctx:KotlinParser.AnnotationListContext):
        pass


    # Enter a parse tree produced by KotlinParser#annotationUseSiteTarget.
    def enterAnnotationUseSiteTarget(self, ctx:KotlinParser.AnnotationUseSiteTargetContext):
        pass

    # Exit a parse tree produced by KotlinParser#annotationUseSiteTarget.
    def exitAnnotationUseSiteTarget(self, ctx:KotlinParser.AnnotationUseSiteTargetContext):
        pass


    # Enter a parse tree produced by KotlinParser#unescapedAnnotation.
    def enterUnescapedAnnotation(self, ctx:KotlinParser.UnescapedAnnotationContext):
        pass

    # Exit a parse tree produced by KotlinParser#unescapedAnnotation.
    def exitUnescapedAnnotation(self, ctx:KotlinParser.UnescapedAnnotationContext):
        pass


    # Enter a parse tree produced by KotlinParser#identifier.
    def enterIdentifier(self, ctx:KotlinParser.IdentifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#identifier.
    def exitIdentifier(self, ctx:KotlinParser.IdentifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#simpleIdentifier.
    def enterSimpleIdentifier(self, ctx:KotlinParser.SimpleIdentifierContext):
        pass

    # Exit a parse tree produced by KotlinParser#simpleIdentifier.
    def exitSimpleIdentifier(self, ctx:KotlinParser.SimpleIdentifierContext):
        pass


    # Enter a parse tree produced by KotlinParser#semi.
    def enterSemi(self, ctx:KotlinParser.SemiContext):
        pass

    # Exit a parse tree produced by KotlinParser#semi.
    def exitSemi(self, ctx:KotlinParser.SemiContext):
        pass


    # Enter a parse tree produced by KotlinParser#anysemi.
    def enterAnysemi(self, ctx:KotlinParser.AnysemiContext):
        pass

    # Exit a parse tree produced by KotlinParser#anysemi.
    def exitAnysemi(self, ctx:KotlinParser.AnysemiContext):
        pass



del KotlinParser